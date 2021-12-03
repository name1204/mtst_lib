/*
 * harmony_default.cpp
 *
 *  Created on: 2021/12/03
 *      Author: matsu
 */

#include "harmony_default.hpp"

namespace harmony_search
{
    namespace harmony_default
    {

        void HarmonySearchStrategy::harmony_initialize()
        {
            using std::size_t;

            this->harmonies_.reserve( this->param_.harmony_size() );

            for ( size_t i = 0, size = this->param_.harmony_size(); i < size; ++i )
            {
                harmonies_.emplace_back( this->generate_init_harmony() );
            }
        }

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

            return Harmony( this->obj_func_( new_harmony_vals ), std::move( new_harmony_vals ) );
        }

        Harmony HarmonySearchStrategy::generate_harmony( const std::size_t ) const
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


        HarmonyResult HarmonyOptimizer::optimize( std::function< double( std::vector< double >& ) > obj_func )
        {
            using std::clock_t;
            using std::vector;

            // 最適化用
            HarmonySearchStrategy strat;

            // 結果記録用
            HarmonyResult result;
            vector< double > update_curve;

            clock_t start = clock();

            for ( std::size_t t = 0; t < 1000; ++t )
            {
                // 更新するハーモニーの選択
                std::size_t index = strat.select_tune_harmony();

                // 新しいハーモニーの生成
                auto new_harmony = strat.generate_harmony( index );

                //ハーモニーメモリ内の評価値と比較して最悪ハーモニーより良ければハーモニーを入れ替える
                strat.trade_harmony( new_harmony );

                // (((注意)))
                // 以前の最良ハーモニーと比較して、より値が良ければ置換を行う
                // これはハーモニーサーチに不要
                // 元々の本名コードでも置換は行うものの最良値は使われていない
            }


            clock_t end = clock();

            return result;
        }
    }    // namespace harmony_default
}    // namespace harmony_search
