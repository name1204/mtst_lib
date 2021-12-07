#include "common_result.hpp"

#include <string>
#include <vector>


using namespace std;
using namespace mtst_common::result;

/* # String format function
 *
 */
template< typename... Args >
std::string format( const std::string& fmt, Args... args )
{
    std::size_t len = static_cast< std::size_t >( snprintf( nullptr, 0, fmt.c_str(), args... ) );
    std::vector< char > buf( len + 1 );
    snprintf( &buf[0], len + 1, fmt.c_str(), args... );
    return std::string( &buf[0], &buf[0] + len );
}

struct Result: public TraitResult
{
    Result() {}

    string csv_header() override
    {
        return string( "Value,Evals,Iter,Time[ms]" );
    }

    string csv_data() override
    {
        return format( "%.15f,%lu,%lu,%.15f\n", this->value_, static_cast< long unsigned int >( this->evals_ ), static_cast< long unsigned int >( this->iter_ ), static_cast< double >( this->time_ ) );
    }
};

void run_TraitResult_tprint_update_curve();

int main( int argc, char** argv )
{
    vector< string > args( argv, argv + argc );

    if ( args.at( 1 ) == string( "HarmonyOptimizer_optimize" ) )
    {
    }

    else
    {
        fprintf( stderr, "Matching test is not exist.\n" );
        exit( -1 );
    }

    return 0;
}


void run_TraitResult_tprint_update_curve()
{
    vector< double > test_data;
    test_data.reserve( 1000 );

    for ( unsigned int i = 1; i < 1000; i++ )
    {
        double t = i * 0.01;
        test_data.emplace_back( t );
    }

    Result result;
    result.set_update_value( test_data )
        .tprint_update_curve( "test_update_curve.csv" );
}