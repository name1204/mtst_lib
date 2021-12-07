/*
 * harmony_default.cpp
 *
 *  Created on: 2021/12/03
 *      Author: matsu
 */

#include "harmony_default.hpp"

namespace harmony_search
{
    namespace hs_default
    {
        HarmonySearchStrategy::HarmonySearchStrategy(
            HarmonySearchParameter param,
            std::size_t dim,
            std::function< double( std::vector< double >& ) > obj_func,
            std::function< std::vector< double >( void ) > init_func,
            std::function< std::vector< double >( void ) > rng_func )
            : param_( param ), dim_( dim ), obj_func_( obj_func ),
              init_generate_func_( init_func ), rng_generate_func_( rng_func )
        {
            using std::size_t;

            this->harmonies_.reserve( this->param_.harmony_size() );

            for ( size_t i = 0, size = this->param_.harmony_size(); i < size; ++i )
            {
                harmonies_.emplace_back( this->generate_init_harmony() );
            }
        }

        HarmonySearchStrategy::HarmonySearchStrategy(
            HarmonySearchParameter param,
            std::size_t dim,
            std::function< double( std::vector< double >& ) > obj_func,
            std::function< std::vector< double >( void ) > init_func )
            : HarmonySearchStrategy( param, dim, obj_func, init_func, [dim]()
                                     { return HarmonySearchStrategy::gen_rng_vals( dim ); } )
        {}

        HarmonySearchStrategy::HarmonySearchStrategy(
            HarmonySearchParameter param,
            std::size_t dim,
            std::function< double( std::vector< double >& ) > obj_func )
            : HarmonySearchStrategy(
                param, dim, obj_func, [dim]()
                { return HarmonySearchStrategy::gen_rng_vals( dim ); },
                [dim]()
                { return HarmonySearchStrategy::gen_rng_vals( dim ); } )
        {}

        HarmonySearchStrategy::HarmonySearchStrategy(
            HarmonySearchParameter param,
            std::size_t dim,
            std::function< double( std::vector< double >& ) > obj_func,
            std::function< std::vector< double >( void ) > init_func,
            std::function< std::vector< double >( void ) > rng_func,
            std::vector< Harmony > harmonies )
            : param_( param ), dim_( dim ), obj_func_( obj_func ),
              init_generate_func_( init_func ), rng_generate_func_( rng_func ),
              harmonies_( harmonies )
        {}

        std::size_t HarmonySearchStrategy::best_harmony() const
        {
            auto minimam = std::min_element( this->harmonies_.begin(), this->harmonies_.end(), [this]( const Harmony& h1, const Harmony& h2 )
                                             { return h1.value() < h2.value(); } );
            return std::distance( this->harmonies_.begin(), minimam );
        }

        std::size_t HarmonySearchStrategy::worst_harmony() const
        {
            auto maximam = std::max_element( this->harmonies_.begin(), this->harmonies_.end(), [this]( const Harmony& h1, const Harmony& h2 )
                                             { return h1.value() < h2.value(); } );
            return std::distance( this->harmonies_.begin(), maximam );
        }

        void HarmonySearchStrategy::trade_harmony( Harmony new_harmony )
        {
            std::size_t index = this->worst_harmony();

            if ( new_harmony.value() < this->harmonies_.at( index ).value() )
            {
                this->harmonies_.at( index ) = new_harmony;
            }
        }

        Harmony HarmonySearchStrategy::generate_tuning_harmony( const std::size_t index ) const
        {
            using std::uniform_real_distribution;

            thread_local std::random_device rnd;                       // 非決定的な乱数生成器を生成
            thread_local std::mt19937 mt( rnd() );                     // メルセンヌ・ツイスタの32ビット版、引数は初期シード値
            uniform_real_distribution<> rng_real( 0.0, 1.0 );          // [0, 1] 範囲の一様乱数
            uniform_real_distribution<> rng_real_abs1( -1.0, 1.0 );    // [-1, 1] 範囲の一様乱数

            std::vector< double > new_harmony_vals;
            new_harmony_vals.reserve( this->dim_ );

            for ( std::size_t i = 0, dim = this->dim_; i < dim; i++ )
            {
                if ( rng_real( mt ) < this->param_.adjustment_ratio() )
                {
                    //値を調整して代入する
                    new_harmony_vals.emplace_back( this->harmonies_.at( index ).harmony().at( i ) + this->param_.band_width() * rng_real_abs1( mt ) );
                }
                else
                {
                    //複製して代入する
                    new_harmony_vals.emplace_back( this->harmonies_.at( index ).harmony().at( i ) );
                }
            }

            return Harmony( this->obj_func_( new_harmony_vals ), new_harmony_vals );
        }

        Harmony HarmonySearchStrategy::generate_harmony() const
        {
            using std::uniform_real_distribution;

            thread_local std::random_device rnd;                 // 非決定的な乱数生成器を生成
            thread_local std::mt19937 mt( rnd() );               // メルセンヌ・ツイスタの32ビット版、引数は初期シード値
            uniform_real_distribution<> rng_real( 0.0, 1.0 );    // [0, 1] 範囲の一様乱数

            //ランダム選択したハーモニーメモリの番号
            std::size_t index = this->select_tune_harmony();

            //新しいハーモニーの生成
            if ( rng_real( mt ) < this->param_.selecttion_ratio() )
            {
                return this->generate_tuning_harmony( index );
            }
            else
            {
                return this->generate_rng_harmony();
            }
        }


        std::size_t HarmonySearchStrategy::select_tune_harmony() const
        {
            thread_local std::random_device rnd;      // 非決定的な乱数生成器を生成
            thread_local std::mt19937 mt( rnd() );    //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
            std::uniform_int_distribution<>
                select_range( 0, static_cast< int >( this->param_.harmony_size() ) - 1 );    // [0, Harmony_size - 1] 範囲の一様乱数
            return select_range( mt );
        }

        std::vector< double > HarmonySearchStrategy::gen_rng_vals( std::size_t dim )
        {
            return HarmonySearchStrategy::gen_rng_vals( dim, 3.0 );
        }

        std::vector< double > HarmonySearchStrategy::gen_rng_vals( std::size_t dim, double range )
        {
            thread_local std::random_device rnd;                                                   // 非決定的な乱数生成器を生成
            thread_local std::mt19937 mt( rnd() );                                                 // メルセンヌ・ツイスタの32ビット版、引数は初期シード値
            std::uniform_real_distribution<> rng_real( -std::abs( range ), std::abs( range ) );    // 一様乱数

            std::vector< double > vals( dim );
            std::generate( vals.begin(), vals.end(), [&]() mutable
                           { return rng_real( mt ); } );
            return vals;
        }

        HarmonyResult HarmonyOptimizer::optimize( std::size_t dim, std::function< double( std::vector< double >& ) > obj_func )
        {
            using std::clock_t;
            using std::vector;

            // 実行時間計測開始
            clock_t start = clock();

            // 初期化アクション
            this->initialize();
            HarmonySearchStrategy strat( this->param_, dim, obj_func );

            // 結果記録用
            vector< double > update_curve;
            update_curve.reserve( strat.param_ref().max_evals() );

            for ( std::size_t t = 0, m_evals = strat.param_ref().max_evals(); t < m_evals; ++t )
            {
                // 更新前アクション
                this->pre_act();

                // 新しいハーモニーの生成
                auto new_harmony = strat.generate_harmony();

                //ハーモニーメモリ内の評価値と比較して最悪ハーモニーより良ければハーモニーを入れ替える
                strat.trade_harmony( new_harmony );

                // 探索履歴の保存
                std::size_t best_index = strat.best_harmony();
                update_curve.emplace_back( strat.harmonies_ref().at( best_index ).value() );

                // 更新後アクション
                this->post_act();
            }

            // 実行時間計測終了
            clock_t end = clock();

            HarmonyResult result;

            std::size_t best_index = strat.best_harmony();
            result
                .set_value( strat.harmonies_ref().at( best_index ).value() )
                .set_variable( strat.harmonies_ref().at( best_index ).harmony() )
                .set_time( end - start )
                .set_update_value( update_curve )
                .set_evals( strat.param_ref().max_evals() + strat.param_ref().harmony_size() );

            return result;
        }
    }    // namespace hs_default
}    // namespace harmony_search
