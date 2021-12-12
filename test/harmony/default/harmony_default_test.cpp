#include "harmony/harmony_default.hpp"

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>


using namespace std;

void run_HarmonyOptimizer_optimize();
void test_HarmonySearchStrategy_detection_best_worst();
void test_HarmonySearchStrategy_trade_harmony();
void run_HarmonySearchStrategy_gen_rng_vals();
void run_HarmonySearchStrategy_generate_harmony_function();

int main( int argc, char** argv )
{
    vector< string > args( argv, argv + argc );

    if ( args.at( 1 ) == string( "HarmonyOptimizer_optimize" ) )
    {
        run_HarmonyOptimizer_optimize();
    }
    else if ( args.at( 1 ) == string( "HarmonySearchStrategy_detection_best_worst" ) )
    {
        test_HarmonySearchStrategy_detection_best_worst();
    }
    else if ( args.at( 1 ) == string( "HarmonySearchStrategy_trade_harmony" ) )
    {
        test_HarmonySearchStrategy_trade_harmony();
    }
    else if ( args.at( 1 ) == string( "HarmonySearchStrategy_gen_rng_vals" ) )
    {
        run_HarmonySearchStrategy_gen_rng_vals();
    }
    else if ( args.at( 1 ) == string( "HarmonySearchStrategy_generate_harmony_function" ) )
    {
        run_HarmonySearchStrategy_generate_harmony_function();
    }
    else
    {
        fprintf( stderr, "Matching test is not exist.\n" );
        exit( -1 );
    }

    return 0;
}

// ベンチマーク関数 Sphere
double sphere( const vector< double >& x )
{
    double value = 0.0;

    for ( auto& v : x )
    {
        value += v * v;
    }

    return value;
}

/* # HarmonyOptimizer_optimize()
 *
 * ミニマルな条件で最適化が実行できるか
 */
void run_HarmonyOptimizer_optimize()
{
    using namespace harmony_search::hs_default;

    auto obj_func = []( const vector< double > x ) -> double
    {
        double value = 0.0;

        for ( auto& v : x )
        {
            value += v * v;
        }

        return value;
    };

    HarmonyOptimizer optimizer( HarmonySearchParameter().set_max_evals( 3000 ) );
    auto result = optimizer.optimize( 10, obj_func );

    printf( "value %.15f\n", result.value() );
    printf( "evals %lu\n", static_cast< long unsigned int >( result.evals() ) );
    printf( "time %.15f\n", static_cast< double >( result.time() ) );
}

/* # HarmonySearchStrategy_detection_best_worst
 *
 * ハーモニーの最良・最悪検出システムが機能しているかどうか
 * チェックする関数
 */
void test_HarmonySearchStrategy_detection_best_worst()
{
    using std::vector;
    using namespace harmony_search::hs_default;

    // genereate defintive hamonies
    vector< Harmony > harmonies {
        Harmony( 0.7, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 7.1, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 1.6, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 2.4, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 0.9, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 0.5, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 6.6, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 3.6, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 4.4, vector< double > { 0.0, 0.0, 0.0 } ),
        Harmony( 5.9, vector< double > { 0.0, 0.0, 0.0 } ),
    };

    // detect best or worst harmony index
    long unsigned int best_index = 0;
    double best_value = harmonies.at( 0 ).value();
    long unsigned int worst_index = 0;
    double worst_value = harmonies.at( 0 ).value();
    for ( std::size_t i = 1, size = harmonies.size(); i < size; ++i )
    {
        if ( harmonies.at( i ).value() < best_value )
        {
            best_index = static_cast< long unsigned int >( i );
            best_value = harmonies.at( i ).value();
        }
        if ( harmonies.at( i ).value() > worst_value )
        {
            worst_index = static_cast< long unsigned int >( i );
            worst_value = harmonies.at( i ).value();
        }
    }

    // generate HarmonyStrategy
    auto strat = HarmonySearchStrategy(
        HarmonySearchParameter().set_max_evals( 3000 ),
        3,
        sphere,
        []()
        { return HarmonySearchStrategy::gen_rng_vals( 3 ); },
        []()
        { return HarmonySearchStrategy::gen_rng_vals( 3 ); },
        harmonies );

    // display result(by HarmonyStrategy)
    printf( "best index : %lu\n", static_cast< long unsigned int >( strat.best_harmony() ) );
    printf( "worst index : %lu\n", static_cast< long unsigned int >( strat.worst_harmony() ) );

    // assert check
    assert( best_index == static_cast< long unsigned int >( strat.best_harmony() ) );
    assert( worst_index == static_cast< long unsigned int >( strat.worst_harmony() ) );
}

/* # HarmonySearchStrategy_trade_harmony
 *
 * 最悪ハーモニーの交換が出来ているか
 * 指定パラメータのハーモニーを入れ、
 * チェックを行う
 */
void test_HarmonySearchStrategy_trade_harmony()
{
    using std::vector;
    using namespace harmony_search::hs_default;
    const unsigned int acc = 1000;    // 10^-n(小数点以下n桁)まで精度を検査


    // genereate defintive hamonies
    vector< Harmony > harmonies {
        Harmony( 0.7, vector< double > {
                          1.2,
                          -2.4,
                          -1.3,
                      } ),
        Harmony( 7.1, vector< double > {
                          1.8,
                          -1.6,
                          0.1,
                      } ),
        Harmony( 1.6, vector< double > {
                          1.0,
                          1.4,
                          0.4,
                      } ),
        Harmony( 2.4, vector< double > {
                          -2.4,
                          -1.3,
                          0.2,
                      } ),
        Harmony( 0.9, vector< double > {
                          -2.0,
                          -0.7,
                          -1.1,
                      } ),
        Harmony( 0.5, vector< double > {
                          -1.6,
                          1.2,
                          0.9,
                      } ),
        Harmony( 6.6, vector< double > {
                          2.2,
                          0.8,
                          0.3,
                      } ),
        Harmony( 3.6, vector< double > {
                          2.3,
                          2.0,
                          0.9,
                      } ),
        Harmony( 4.4, vector< double > {
                          -0.1,
                          -1.8,
                          1.0,
                      } ),
        Harmony( 5.9, vector< double > {
                          0.4,
                          -0.7,
                          1.0,
                      } ),
    };

    // change chandidate
    Harmony should_not_change( 9.4, vector< double > {
                                        0.3,
                                        -2.4,
                                        1.9,
                                    } );
    Harmony should_change( 0.2, vector< double > {
                                    0.4,
                                    -0.7,
                                    1.0,
                                } );
    // generate HarmonyStrategy
    auto strat = HarmonySearchStrategy(
        HarmonySearchParameter().set_max_evals( 3000 ),
        3,
        sphere,
        []()
        { return HarmonySearchStrategy::gen_rng_vals( 3 ); },
        []()
        { return HarmonySearchStrategy::gen_rng_vals( 3 ); },
        harmonies );

    // should not change test
    Harmony worst_harmony = strat.harmonies_ref().at( strat.worst_harmony() );
    strat.trade_harmony( should_not_change );
    printf( "%f\n", strat.harmonies_ref().at( strat.worst_harmony() ).value() );
    assert( static_cast< long int >( std::round( strat.harmonies_ref().at( strat.worst_harmony() ).value() * acc ) ) == static_cast< long int >( std::round( worst_harmony.value() * acc ) ) );

    // should change test
    worst_harmony = strat.harmonies_ref().at( strat.worst_harmony() );
    strat.trade_harmony( should_change );
    printf( "%f\n", strat.harmonies_ref().at( strat.worst_harmony() ).value() );
    assert( static_cast< long int >( std::round( strat.harmonies_ref().at( strat.worst_harmony() ).value() * acc ) ) != static_cast< long int >( std::round( worst_harmony.value() * acc ) ) );
}

/* # HarmonySearchStrategy
 *
 * gen_rng_valsで乱数変数列が出来ているか
 * チェックを行う
 */
void run_HarmonySearchStrategy_gen_rng_vals()
{
    using namespace harmony_search::hs_default;

    printf( "\ndefault geneartion\n" );
    for ( size_t i = 0; i < 10; ++i )
    {
        auto vals = HarmonySearchStrategy::gen_rng_vals( 5 );
        for ( auto& x : vals )
        {
            printf( "% 5.5f ", x );
        }
        printf( "\n" );
    }

    printf( "\n\nspecific geneartion\n" );
    for ( size_t i = 0; i < 10; ++i )
    {
        auto vals = HarmonySearchStrategy::gen_rng_vals( 5, 10.0 );
        for ( auto& x : vals )
        {
            printf( "% 5.5f ", x );
        }
        printf( "\n" );
    }
}

void run_HarmonySearchStrategy_generate_harmony_function()
{
    using namespace harmony_search::hs_default;

    HarmonySearchStrategy strat( HarmonySearchParameter().set_max_evals( 3000 ), 3, sphere );

    {
        printf( "\nwhole harmony generation\n" );
        for ( size_t i = 0; i < 5; ++i )
        {
            auto harmony = strat.generate_harmony();
            printf( "value : %.15f\nvals : ", harmony.value() );
            for ( auto& x : harmony.harmony() )
            {
                printf( "% 5.5f ", x );
            }
            printf( "\n" );
        }
    }
    {
        printf( "\ninit harmony generation\n" );
        for ( size_t i = 0; i < 5; ++i )
        {
            auto harmony = strat.generate_init_harmony();
            printf( "value : %.15f\nvals : ", harmony.value() );
            for ( auto& x : harmony.harmony() )
            {
                printf( "% 5.5f ", x );
            }
            printf( "\n" );
        }
    }
    {
        printf( "\nrng harmony generation\n" );
        for ( size_t i = 0; i < 5; ++i )
        {
            auto harmony = strat.generate_rng_harmony();
            printf( "value : %.15f\nvals : ", harmony.value() );
            for ( auto& x : harmony.harmony() )
            {
                printf( "% 5.5f ", x );
            }
            printf( "\n" );
        }
    }
    {
        printf( "\ntuning harmony generation\n" );
        for ( size_t i = 0; i < 5; ++i )
        {
            auto harmony = strat.generate_tuning_harmony( 0 );
            printf( "value : %.15f\nvals : ", harmony.value() );
            for ( auto& x : harmony.harmony() )
            {
                printf( "% 5.5f ", x );
            }
            printf( "\n" );
        }
    }
}