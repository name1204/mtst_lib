/*
 * thread_pool.hpp
 *
 *  Created on: 2021/04/24
 *      Author: matsu
 */

#ifndef THREAD_POOL_HPP_
#define THREAD_POOL_HPP_

#include <queue>
#include <vector>

#include <algorithm>
#include <functional>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>


namespace CellTp //cell of thread pool
{
	/* indexing_sequence
	 *   for std++11
	 */
	namespace indexing_sequence
	{
		template<std::size_t...> struct index_sequence {};

		template<std::size_t N, bool> struct P { template<std::size_t... Is> index_sequence<Is..., (Is+N)...> f(index_sequence<Is...>); };
		template<std::size_t N> struct P<N, true> { template<std::size_t... Is> index_sequence<Is..., (Is+N)..., N*2> f(index_sequence<Is...>); };

		template<std::size_t N> struct Q { decltype(P<N/2, N%2>{}.f(Q<N/2>{}.g())) g(); };
		template<> struct Q<0> { index_sequence<> g(); };

		template<std::size_t N> using make_index_sequence = decltype(Q<N>{}.g());
		template <class... T> using index_sequence_for = make_index_sequence<sizeof...(T)>;
	}

	namespace thread_pool
	{
		struct ThreadPool;
		namespace tp_inner
		{
			static thread_local std::once_flag tp_init_flag;
			static thread_local ThreadPool* tp = static_cast<ThreadPool*>(0);
		}

		enum class PoolSize
		{
			single, half, full
		};

		struct ThreadPool
		{
		protected:
			std::size_t num_workers;
			std::vector<std::thread> workers;

			std::queue<std::function<void(void)>> tasks;
			std::mutex tasks_mutex;
			std::condition_variable tasks_cv;

			bool active;
			bool stop;

			std::mutex join_mutex;
			std::condition_variable join_cv;
			std::vector<bool> working;

			void activate(const unsigned int);
			void shutdown();

		public:
			ThreadPool()
				: active(false), stop(false)
			{
				this->num_workers = std::thread::hardware_concurrency() - 1;
				working.resize(num_workers, false);
				for (std::size_t i = 0; i < num_workers; ++i)
				{
					workers.emplace_back([this, i]
									 { activate(i); });
				}
			}

			ThreadPool(bool full)
				: active(false), stop(false)
			{
				if(full)
				{
					this->num_workers = std::thread::hardware_concurrency();
				}
				else
				{
					this->num_workers = std::thread::hardware_concurrency() - 1;
				}
				working.resize(num_workers, false);

				for (std::size_t i = 0; i < num_workers; ++i)
				{
					workers.emplace_back([this, i]
									 { activate(i); });
				}
			}

			ThreadPool(std::size_t num_threads)
				: num_workers(num_threads), active(false), stop(false)
			{
				working.resize(num_workers, false);
				for (std::size_t i = 0; i < num_workers; ++i)
				{
					workers.emplace_back([this, i]
									 { activate(i); });
				}
			}

			virtual ~ThreadPool()
			{
				if (!stop)
				{
					shutdown();
				}
			}

			std::size_t num_threads() const
			{ return this->num_workers; }

			void request(std::function<void(void)>);
			template<class F, class... Args>
			auto spawn(F&& f, Args&&... args)
				-> std::future<typename std::result_of<F(Args...)>::type>;
//		template<typename F, typename...Args>
//		auto submit(F&& f, Args&&... args)
//			-> std::future<decltype(f(args...))>;
			void join();
		};

		inline void ThreadPool::request(std::function<void(void)> f)
		{
			active = true;
			{
				std::unique_lock<std::mutex> lock(tasks_mutex);
				tasks.emplace(f);
			}
			tasks_cv.notify_one();
		}

		template<class F, class... Args>
		inline auto ThreadPool::spawn(F&& f, Args&&... args)
			-> std::future<typename std::result_of<F(Args...)>::type>
		{
			using return_type = typename std::result_of<F(Args...)>::type;

			auto task =
				std::make_shared< std::packaged_task<return_type()> >
				(
					std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);

			std::future<return_type> res = task->get_future();
/*	    {
	        std::unique_lock<std::mutex> lock(tasks_mutex);

	        // don't allow enqueueing after stopping the pool
	        if(stop)
	            throw std::runtime_error("enqueue on stopped ThreadPool");

	        tasks.emplace([task](){ (*task)(); });
	    }
	    tasks_cv.notify_one();*/

			this->request([task](){ (*task)(); });

			return res;
		}
/*
	template<typename F, typename...Args>
	inline auto ThreadPool::submit(F&& f, Args&&... args)
		-> std::future<decltype(f(args...))>
	{
		// Create a function with bounded parameters ready to execute
		std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		// Encapsulate it into a shared ptr in order to be able to copy construct / assign
		auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

		{
			std::unique_lock<std::mutex> lock(tasks_mutex);

	        // don't allow enqueueing after stopping the pool
	        if(stop)
	            throw std::runtime_error("enqueue on stopped ThreadPool");

	        tasks.emplace([task](){ (*task)(); });
	    }
	    tasks_cv.notify_one();
	    return task->get_future();
	}
*/
		inline void ThreadPool::join()
		{
			if(!active)
			{
				return;
			}

			tasks_cv.notify_all();

			while(true)
			{
				std::unique_lock<std::mutex> lock(join_mutex);
				join_cv.wait(lock, [this]
					{
						bool is_working = std::none_of(working.begin(), working.end(), [](bool x) { return x; });
						return active && tasks.empty() && is_working;
					});
				break;
			}
			active = false;
		}

		inline void ThreadPool::activate(const unsigned int id)
		{
			using namespace tp_inner;

			while(true)
			{
				std::function<void(void)> task;
				{
					std::unique_lock<std::mutex> lock(tasks_mutex);
					tasks_cv.wait(lock,
						[this](){ return !tasks.empty() || stop; });

					if (stop && tasks.empty())
						return;
					{
						std::unique_lock<std::mutex> lock(join_mutex);
						working.at(id) = true;
					}
					task = std::move(tasks.front());
					tasks.pop();
				}
				task();
				{
					std::unique_lock<std::mutex> lock(join_mutex);
					working.at(id) = false;
					if(std::none_of(working.begin(), working.end(), [](bool x) { return x; }))
						join_cv.notify_all();
				}
			}

			delete tp;
		}

		inline void ThreadPool::shutdown()
		{
			{
				std::unique_lock<std::mutex> lock(tasks_mutex);
				stop = true;
			}
			tasks_cv.notify_all();
			for (std::thread& worker :workers)
			{
				worker.join();
			}
		}
	}

	template<class F, class... Args>
	auto spawn(F&& f, Args&&... args)
	    -> std::future<typename std::result_of<F(Args...)>::type>
	{
		using namespace thread_pool;
		using namespace tp_inner;

		std::call_once(tp_init_flag,
			[]()
			{
				tp = new ThreadPool(true);
			}
		);

	    return tp->spawn(std::forward<F>(f), std::forward<Args>(args)...);
	}

	template<class Iterator, class Function>
	inline Function parallel_for_each(Iterator begin, Iterator end, Function f)
	{
		using namespace thread_pool;
		using namespace tp_inner;

		std::call_once( tp_init_flag,
			[]()
			{
				tp = new ThreadPool(true);
			}
		);

		thread_local const std::size_t num_threads = tp->num_threads();

		if(begin == end)
		{
			return std::move(f);
		}

		std::size_t step = std::max<std::size_t>(1, std::distance(begin, end) / num_threads);

		for(; begin < end - step; begin += step)
		{
			tp->request
				([=, &f](){std::for_each(begin, begin + step, f);} );
		}
		tp->request([=, &f](){std::for_each(begin, end, f);} );

		tp->join();

		return std::move(f);
	}

	template<class Container, class Function>
	inline Function parallel_for_each(Container&& c, Function f)
	{
		return parallel_for_each(std::begin(c), std::end(c), f);
	}

	template<class Function, class X, typename Tuple, std::size_t... I>
	inline Function zip_for_each_impl
		(Function& f, X& x, Tuple& t, indexing_sequence::index_sequence<I...>)
	{
		 f(x, *std::get<I>(t)...);

		 using swallow = std::initializer_list<int>;
		  (void)swallow{ ((void)++std::get<I>(t), 0)... };

		return std::move(f);
	}

	template<class Function, class Container, class... Containers>
	inline Function zip_for_each(Function&& f, Container&& ctr, Containers&&... ctrs)
	{
	#ifdef _DEBUG
		std::size_t ctr_len = ctr.size();
		std::vector<std::size_t> len{ ctrs.size()... };
		bool is_same_len = std::all_of(len.begin(), len.end(), [&](std::size_t x){ return x == ctr_len; } );
		if(!is_same_len)
		{
			fprintf(stderr,
					"Error: [%s l.%d]Input length is not same.(zip_for_each)\n",
					__FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}
	#endif
	    using ctr_valuetype = typename std::remove_reference<decltype(ctr)>::type::value_type;

		auto t = std::make_tuple(ctrs.begin()...);
		auto id = indexing_sequence::make_index_sequence<sizeof...(Containers)>();
		for_each(ctr.begin(), ctr.end(),
			[&](ctr_valuetype& v)
			{
				zip_for_each_impl(f, v, t, id);
			}
		);

		return std::move(f);
	}

	template<class Function, class Container, class... Containers>
	inline Function zip_parallel_for_each(Function&& f, Container&& ctr, Containers&&... ctrs)
	{
	#ifdef _DEBUG
		std::size_t ctr_len = ctr.size();
		std::vector<std::size_t> len{ ctrs.size()... };
		bool is_same_len = std::all_of(len.begin(), len.end(), [&](std::size_t x){ return x == ctr_len; } );
		if(!is_same_len)
		{
			fprintf(stderr,
					"Error: [%s l.%d]Input length is not same.(zip_parallel_for_each)\n",
					__FILE__, __LINE__);
			exit(EXIT_FAILURE);
		}
	#endif
	    using ctr_valuetype = typename std::remove_reference<decltype(ctr)>::type::value_type;

		auto t = std::make_tuple(ctrs.begin()...);
		auto id = indexing_sequence::make_index_sequence<sizeof...(Containers)>();
		parallel_for_each(ctr.begin(), ctr.end(),
			[&](ctr_valuetype& v)
			{
				zip_for_each_impl(f, v, t, id);
			}
		);

		return std::move(f);
	}
}

namespace TestCellTp
{
	void test_zip_for_each()
	{
		std::vector<double> x{0.0, 0.0, 0.0};
		std::vector<double> v{2.2, 1.0, 2.8};
		std::vector<double> px{3.2, 1.4, 2.8};
		std::vector<double> gx{3.8, 5.0, 4.9};

		CellTp::zip_for_each(
			[](double& x, double& y, double& z, double& v)
			{
				printf("%f, %f, %f, %f\n", x, y, z, v);
				x = y + z +v;
			},
			x, v, px, gx);

		for(auto inx :x)
		printf("%f ", inx);
	}
}

#endif /* THREAD_POOL_HPP_ */
