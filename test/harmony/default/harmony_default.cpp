#include <cstdio>
#include <string>
#include <vector>


using namespace std;

void test_HarmonySearchStrategy_ranked_harmony();

int main( int argc, char** argv )
{
    vector< string > args( argv, argv + argc );

    if ( args.at( 1 ) == string( "BandParam_new" ) )
    {
        test_HarmonySearchStrategy_ranked_harmony();
    }
    else
    {
        fprintf( stderr, "Matching test is not exist.\n" );
        exit( -1 );
    }

    return 0;
}