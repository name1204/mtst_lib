/*
 * harmony_default.hpp
 *
 *  Created on: 2021/12/03
 *      Author: matsu
 */

#ifndef INCLUDE_HARMONY_DEFAULT_HPP_
#define INCLUDE_HARMONY_DEFAULT_HPP_

#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <string>
#include <vector>

namespace harmony_search
{
    namespace harmony_default
    {
        struct Harmony
        {
        protected:

            double value_;
            std::vector< double > harmony_;

        public:

            Harmony(): value_( 0 ) {}
            Harmony( double value, std::vector< double > vals )
                : value_( value ), harmony_( vals )
            {}

            std::vector< double > harmony() const
            {
                return harmony_;
            }
            double value() const
            {
                return value_;
            }
            Harmony& set_harmony( std::vector< double > input )
            {
                harmony_ = input;
                return *this;
            }
            Harmony& value( double input )
            {
                value_ = input;
                return *this;
            }
        };

        struct HarmonySearchParameter
        {
        protected:

            std::size_t max_iter_;        //更新回数
            std::size_t harmony_size_;    //ハーモニーメモリのサイズ
            double band_width_;           //バンド幅
            double selecttion_ratio_;     // = Ra = 選択比率(大きければ大きい程、ハーモニーメモリ内が使われる)
            double adjustment_ratio_;     // = Rp = 値調整比率(大きければ大きい程、値を調整する確率が上がる)

        public:

            HarmonySearchParameter()
                : max_iter_( 9999990 ), harmony_size_( 10 ), band_width_( 0.1 ),
                  selecttion_ratio_( 0.9 ), adjustment_ratio_( 0.1 )
            {}

            std::size_t max_iter() const
            {
                return max_iter_;
            }
            size_t harmony_size() const
            {
                return harmony_size_;
            }
            double band_width() const
            {
                return band_width_;
            }
            double selecttion_ratio() const
            {
                return selecttion_ratio_;
            }
            double adjustment_ratio() const
            {
                return adjustment_ratio_;
            }

            HarmonySearchParameter& max_iter( std::size_t input )
            {
                max_iter_ = input;
                return *this;
            }
            HarmonySearchParameter& harmony_size( std::size_t input )
            {
                harmony_size_ = input;
                return *this;
            }
            HarmonySearchParameter& band_width( double input )
            {
                band_width_ = input;
                return *this;
            }
            HarmonySearchParameter& selecttion_ratio( double input )
            {
                selecttion_ratio_ = input;
                return *this;
            }
            HarmonySearchParameter& adjustment_ratio( double input )
            {
                adjustment_ratio_ = input;
                return *this;
            }
        };

        struct HarmonySearchStrategy
        {
        protected:

            HarmonySearchParameter param_;
            std::vector< Harmony > harmonies_;
            std::size_t worst_harmony_;    //最悪ハーモニーの番号
            std::size_t best_harmony_;     //最良ハーモニーの番号

            std::size_t dim_;
            std::function< double( std::vector< double >& ) > obj_func_;
            std::function< Harmony( void ) > init_generate_func_;
            std::function< Harmony( void ) > rng_generate_func_;

        public:

            HarmonySearchStrategy()
            {}

            void harmony_initialize();
            Harmony generate_init_harmony() const
            {
                return this->init_generate_func_();
            }
            Harmony generate_rng_harmony() const
            {
                return this->rng_generate_func_();
            }
            Harmony generate_tuning_harmony( const std::size_t ) const;
            Harmony generate_harmony( const std::size_t ) const;
            std::size_t best_harmony() const;
            std::size_t worst_harmony() const;
            void trade_harmony( Harmony );

            std::size_t select_tune_harmony() const;
        };

        struct HarmonyResult
        {
        protected:

            double value_;    //最良評価値
            std::vector< double > update_value_;
            std::vector< double > variable_;         //最良評価値の時の変数
            std::vector< double > init_variable_;    // 最適化開始時の最良評価値の変数
            std::vector< std::vector< double > > update_variable_;
            double evals_;         //評価回数
            std::clock_t time_;    // 1試行あたりの最適化の実行時間
            size_t iter_;          //更新回数

        public:

            HarmonyResult() {}

            HarmonyResult& set_value( double input )
            {
                value_ = input;
                return *this;
            }

            HarmonyResult& set_update_value( std::vector< double > input )
            {
                update_value_ = input;
                return *this;
            }

            HarmonyResult& set_variable( std::vector< double > input )
            {
                variable_ = input;
                return *this;
            }

            HarmonyResult& set_init_variable( std::vector< double > input )
            {
                init_variable_ = input;
                return *this;
            }

            HarmonyResult& set_update_variable( std::vector< std::vector< double > > input )
            {
                update_variable_ = input;
                return *this;
            }

            HarmonyResult& set_evals( double input )
            {
                evals_ = input;
                return *this;
            }

            HarmonyResult& set_iter( size_t input )
            {
                iter_ = input;
                return *this;
            }

            HarmonyResult& set_time( std::clock_t input )
            {
                time_ = input;
                return *this;
            }

            HarmonyResult& set_time( double input )
            {
                time_ = static_cast< std::clock_t >( input );
                return *this;
            }

            double value() const
            {
                return value_;
            }
            std::vector< double > update_value() const
            {
                return update_value_;
            }
            std::vector< double > variable() const
            {
                return variable_;
            }
            std::vector< double > init_variable() const
            {
                return init_variable_;
            }
            std::vector< std::vector< double > > update_variable() const
            {
                return update_variable_;
            }
            double evals() const
            {
                return evals_;
            }
            std::clock_t time() const
            {
                return time_;
            }
            size_t iter() const
            {
                return iter_;
            }
        };

        struct HarmonyOptimizer
        {
            HarmonyOptimizer( HarmonySearchParameter );
            HarmonyResult optimize( std::function< double( std::vector< double >& ) > );
        };
    }    // namespace harmony_default
}    // namespace harmony_search


#endif /* INCLUDE_HARMONY_DEFAULT_HPP_ */
