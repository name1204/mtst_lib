#include "harmony_default.hpp"

#include <cstdio>
#include <string>
#include <vector>


using namespace std;

void run_HarmonyOptimizer_optimize();

int main( int argc, char** argv )
{
    vector< string > args( argv, argv + argc );

    if ( args.at( 1 ) == string( "HarmonyOptimizer_optimize" ) )
    {
        run_HarmonyOptimizer_optimize();
    }
    else
    {
        fprintf( stderr, "Matching test is not exist.\n" );
        exit( -1 );
    }

    return 0;
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

    printf( "%.15f\n", result.value() );
}