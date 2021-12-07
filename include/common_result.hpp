/*
 * common_result.hpp
 *
 *  Created on: 2021/12/07
 *      Author: matsu
 */

#ifndef INCLUDE_COMMON_RESULT_HPP_
#define INCLUDE_COMMON_RESULT_HPP_

#include <chrono>
#include <cmath>
#include <string>
#include <vector>

namespace mtst_common
{
    namespace result
    {
        /* # Result の最良変数列をCSV出力する
         *
         * 全試行分の最良値の変数をまとめる関数
         * ヘッダー : No,variables,...
         *
         * # template
         *
         *   R : `R.variables()` が呼べること(CommonResultを継承すればOK)
         *
         * # 引数
         *
         *   results :vector<R> 全試行分の結果の配列
         *   file_name :string 出力するファイル名(パスを含んだファイル名)。拡張子を含めること。
         */
        template< class R >
        void tprint_all_variables( const std::vector< R >& results, const std::string& file_name )
        {
            FILE* csv = fopen( file_name.c_str(), "w" );

            // header 印字
            fprintf( csv, "No," );
            for ( std::size_t i = 0, len = results.at( 0 ).variables().size(); i < len; ++i )
            {
                fprintf( csv, "Variable[%d],", i );
            }
            fprintf( csv, "\n" );

            // 係数値印字
            for ( std::size_t i = 0, len = results.size(); i < len; ++i )
            {
                // No
                fprintf( csv, "%d,", i + 1 );

                // Variables
                for ( std::size_t j = 0, dim = results.at( i ).get_variable().size() - 1; j < dim; j++ )
                {
                    fprintf( csv, "%.15f,", results.at( i ).get_variable().at( j ) );
                }
                fprintf( csv, "%.15f\n", results.at( i ).get_variable().back() );
            }

            fclose( csv );
        }

        /* # Result のデータをCSV出力する
         *
         * 全試行分のデータをまとめて処理する関数
         * ヘッダー : ``R.csv_header();``
         *
         * # template
         *
         *   R : `R.csv_header()` , `R.csv_data()`が呼べること
         *   CommonResultを継承し、`R.csv_header()` , `R.csv_data()`を実装する
         *
         * # 引数
         *
         *   results :vector<R> 全試行分の結果の配列
         *   file_name :string 出力するファイル名(パスを含んだファイル名)。拡張子を含めること。
         */
        template< class R >
        void tprint_all_result( const std::vector< R >& results, const std::string& filename )
        {
            if ( results.size() <= 0 )
            {
                fprintf( stderr, "Error: [%s l.%d] Vector's length is 0.\n", __FILE__, __LINE__ );
                exit( EXIT_FAILURE );
            }

            FILE* csv = fopen( filename.c_str(), "w" );

            // header 印字
            fprintf( csv, "No,%s\n", results.at( 0 ).csv_header().c_str() );

            // データ印字
            for ( std::size_t i = 0, len = results.size(); i < len; ++i )
            {
                fprintf( csv, "%lu,", static_cast< long unsigned int >( i ) );
                fprintf( csv, "%s\n", results.at( i ).csv_data().c_str() );
            }

            fclose( csv );
        }

        struct TraitResult
        {
        protected:

            double value_;    //最良評価値
            std::vector< double > update_value_;
            std::vector< double > variable_;         //最良評価値の時の変数
            std::vector< double > init_variable_;    // 最適化開始時の最良評価値の変数
            std::vector< std::vector< double > > update_variable_;
            std::size_t evals_;    //評価回数
            std::clock_t time_;    // 1試行あたりの最適化の実行時間
            std::size_t iter_;     //更新回数

        public:

            TraitResult(): value_( std::nan( "uninitalize value" ) ), evals_( 0 ), iter_( 0 ) {}
            virtual ~TraitResult() {}

            TraitResult& set_value( double input )
            {
                value_ = input;
                return *this;
            }

            TraitResult& set_update_value( std::vector< double > input )
            {
                update_value_ = input;
                return *this;
            }

            TraitResult& set_variable( std::vector< double > input )
            {
                variable_ = input;
                return *this;
            }

            TraitResult& set_init_variable( std::vector< double > input )
            {
                init_variable_ = input;
                return *this;
            }

            TraitResult& set_update_variable( std::vector< std::vector< double > > input )
            {
                update_variable_ = input;
                return *this;
            }

            TraitResult& set_evals( std::size_t input )
            {
                evals_ = input;
                return *this;
            }

            TraitResult& set_iter( size_t input )
            {
                iter_ = input;
                return *this;
            }

            TraitResult& set_time( clock_t input )
            {
                time_ = input;
                return *this;
            }

            TraitResult& set_time( double input )
            {
                time_ = static_cast< clock_t >( input );
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
            std::vector< double > variables() const
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
            std::size_t evals() const
            {
                return evals_;
            }
            std::clock_t time() const
            {
                return time_;
            }
            std::size_t iter() const
            {
                return iter_;
            }

            void tprint_update_curve( const std::string& ) const;
            void gprint_update_curve( const std::string&, const bool ) const;
            void tprint_update_variable( const std::string& ) const;
            void tprint_variable( const std::string& ) const;
            void tprint_init_variable( const std::string& ) const;

            virtual std::string csv_header() = 0;
            virtual std::string csv_data() = 0;
        };
    }    // namespace result
}    // namespace mtst_common


#endif /* INCLUDE_COMMON_RESULT_HPP_ */
