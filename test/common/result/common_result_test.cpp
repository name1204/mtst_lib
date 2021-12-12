#include "common/common_result.hpp"

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

    string csv_header() const override
    {
        return string( "Value,Evals,Iter,Time[ms]" );
    }

    string csv_data() const override
    {
        return format( "%.15f,%lu,%lu,%.15f", this->value_, static_cast< long unsigned int >( this->evals_ ), static_cast< long unsigned int >( this->iter_ ), static_cast< double >( this->time_ ) );
    }
};

void run_TraitResult_tprint_update_curve();
void run_TraitResult_gprint_update_curve();
void run_TraitResult_tprint_update_variable();
void run_TraitResult_tprint_variable();
void run_TraitResult_tprint_init_variable();
void run_tprint_all_result();
void run_tprint_all_variables();


int main( int argc, char** argv )
{
    vector< string > args( argv, argv + argc );

    if ( args.at( 1 ) == string( "TraitResult_tprint_update_curve" ) )
    {
        run_TraitResult_tprint_update_curve();
    }
    else if ( args.at( 1 ) == string( "TraitResult_gprint_update_curve" ) )
    {
        run_TraitResult_gprint_update_curve();
    }
    else if ( args.at( 1 ) == string( "TraitResult_tprint_update_variable" ) )
    {
        run_TraitResult_tprint_update_variable();
    }
    else if ( args.at( 1 ) == string( "TraitResult_tprint_variable" ) )
    {
        run_TraitResult_tprint_variable();
    }
    else if ( args.at( 1 ) == string( "TraitResult_tprint_init_variable" ) )
    {
        run_TraitResult_tprint_init_variable();
    }
    else if ( args.at( 1 ) == string( "tprint_all_result" ) )
    {
        run_tprint_all_result();
    }
    else if ( args.at( 1 ) == string( "tprint_all_variables" ) )
    {
        run_tprint_all_variables();
    }
    else
    {
        fprintf( stderr, "Matching test is not exist.\n" );
        exit( -1 );
    }

    return 0;
}

/* # TraitResult
 *
 * 抽象クラスの関数について実行テストを行う
 *　`tprint_update_curve`
 */
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

/* # TraitResult
 *
 * 抽象クラスの関数について実行テストを行う
 * `gprint_update_curve`
 *
 * 縦軸を対数にするものとしないものの2種テストする
 */
void run_TraitResult_gprint_update_curve()
{
    vector< double > test_data;
    test_data.reserve( 1000 );

    for ( unsigned int i = 1; i < 1000; i++ )
    {
        double t = i * 0.01;
        test_data.emplace_back( t );
    }

    Result result;
    result.set_update_value( test_data );

    // nomal scale
    result.gprint_update_curve( string( "test_upgate_curve.png" ), false );
    // log scale
    result.gprint_update_curve( string( "test_upgate_curve_log.png" ), true );
}

/* # TraitResult
 *
 * 抽象クラスの関数について実行テストを行う
 * `tprint_update_variable`
 *
 */
void run_TraitResult_tprint_update_variable()
{
    vector< vector< double > > test_data;

    for ( unsigned int i = 1; i < 51; i++ )
    {
        vector< double > data { 0.1 * i, 0.2 * i, 0.3 * i };
        test_data.emplace_back( data );
    }

    Result result;
    result.set_update_variable( test_data )
        .tprint_update_variable( string( "test_update_variables.csv" ) );
}

/* # TraitResult
 *
 * 抽象クラスの関数について実行テストを行う
 * `tprint_variable`
 *
 */
void run_TraitResult_tprint_variable()
{
    vector< double > test_data;
    test_data.reserve( 20 );

    for ( unsigned int i = 1; i < 200; i++ )
    {
        double t = i * 0.01;
        test_data.emplace_back( t );
    }

    Result result;
    result.set_variable( test_data )
        .tprint_variable( string( "test_variables.csv" ) );
}

/* # TraitResult
 *
 * 抽象クラスの関数について実行テストを行う
 * `tprint_init_variable`
 *
 */
void run_TraitResult_tprint_init_variable()
{
    vector< vector< double > > test_data;

    for ( unsigned int i = 1; i < 101; i++ )
    {
        vector< double > data { 0.3 * i, 0.5 * i, 0.7 * i };
        test_data.emplace_back( data );
    }

    Result result;
    result.set_init_variable( test_data )
        .tprint_init_variable( string( "test_init_variables.csv" ) );
}

/* # tprint_all_result
 *
 * 実行テスト
 */
void run_tprint_all_result()
{
    vector< Result > results;

    for ( unsigned int i = 0; i < 51; ++i )
    {
        Result result;
        result.set_value( 0.1 * i )
            .set_variable( vector< double > { 0.3 * i, 0.5 * i, 0.7 * i } )
            .set_iter( 100 )
            .set_evals( 100 * 100 )
            .set_time( 950.0 );

        results.emplace_back( result );
    }

    tprint_all_result( results, string( "test_all_results.csv" ) );
}

/* # tprint_all_variables
 *
 * 実行テスト
 */
void run_tprint_all_variables()
{
    vector< Result > results;

    for ( unsigned int i = 0; i < 51; ++i )
    {
        Result result;
        result.set_value( 0.1 * i )
            .set_variable( vector< double > { 0.3 * i, 0.5 * i, 0.7 * i } )
            .set_iter( 100 )
            .set_evals( 100 * 100 )
            .set_time( 950.0 );

        results.emplace_back( result );
    }

    tprint_all_variables( results, string( "test_all_variables.csv" ) );
}