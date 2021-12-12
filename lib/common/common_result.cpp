/*
 * common_result.cpp
 *
 *  Created on: 2021/12/07
 *      Author: matsu
 */

#include "common/common_result.hpp"

namespace mtst_common
{
    namespace result
    {
        /*
         *  更新曲線
         *  評価値の推移を示す
         *
         * # 引数
         *   file_name :string 出力するファイル名。拡張子を含めること。
         */
        void TraitResult::tprint_update_curve( const std::string& file_name ) const
        {
            FILE* csv = fopen( file_name.c_str(), "w" );

            // header 印字
            fprintf( csv, "No,Value\n" );

            // 係数値印字
            for ( unsigned int i = 0; i < this->update_value_.size(); i++ )
            {
                fprintf( csv, "%d,%.15f\n", i + 1, this->update_value_.at( i ) );
            }

            fclose( csv );
        }

        /*
         * 更新曲線 画像出力
         *
         * # 引数
         *   file_name :string 出力するファイル名。拡張子を含めること。
         *   logscale :bool y軸を対数で取るか選択 true:取る false:取らない
         */
        void TraitResult::gprint_update_curve( const std::string& file_name, bool logscale ) const
        {
            // xrangeの右端を切り上げる
            unsigned int digit = static_cast< unsigned int >( std::floor( std::log10( this->update_value_.size() ) ) );
            const double digit_pow = std::pow( 10.0, digit );
            unsigned int xrange = static_cast< unsigned int >( std::ceil( static_cast< double >( this->update_value_.size() ) / digit_pow ) * digit_pow );

            // gnuplotで出力
            FILE* gp = popen( "gnuplot -persist", "w" );
            fprintf( gp, "set terminal pngcairo size 1280, 960\n" );
            fprintf( gp, "set output '%s'\n", file_name.c_str() );
            fprintf( gp, "set grid\n" );
            fprintf( gp, "set xlabel 'Number of evaluations'\n" );
            fprintf( gp, "set ylabel 'Objective function value'\n" );
            fprintf( gp, "set key    font 'Times New Roman,15'\n" );
            fprintf( gp, "set xlabel font 'Times New Roman,20'\n" );
            fprintf( gp, "set ylabel font 'Times New Roman,20'\n" );
            fprintf( gp, "set tics   font 'Times New Roman,15'\n" );
            fprintf( gp, "set lmargin 20\n" );
            fprintf( gp, "set bmargin 10\n" );
            fprintf( gp, "set xrange [0:%d]\n", xrange );
            // fprintf(gp, "set yrange [0:10]\n");
            if ( logscale )
            {
                fprintf( gp, "set logscale y \n" );
            }
            fprintf( gp, "plot '-' with lines title \"\n" );

            for ( std::size_t i = 0, len = this->update_value_.size(); i < len; ++i )
            {
                fprintf( gp, "%lu %.15f\n", static_cast< long unsigned int >( i + 1 ), this->update_value_.at( i ) );
            }

            fprintf( gp, "e\n" );
            pclose( gp );
        }

        /*
         * # 変数の更新経歴出力
         *
         * 変数の変遷をCSV形式で出力する
         *
         * # 引数
         *   file_name :string 出力するファイル名。拡張子を含めること。
         */
        void TraitResult::tprint_update_variable( const std::string& file_name ) const
        {
            FILE* csv = fopen( file_name.c_str(), "w" );

            // header 印字
            fprintf( csv, "No," );
            for ( std::size_t i = 0, len = this->update_variable_.at( 0 ).size(); i < len; i++ )
            {
                fprintf( csv, "Variable[%lu],", static_cast< long unsigned int >( i ) );
            }
            fprintf( csv, "\n" );

            // 係数値印字
            for ( std::size_t i = 0, len = this->update_variable_.size(); i < len; ++i )
            {
                // No
                fprintf( csv, "%lu,", static_cast< long unsigned int >( i + 1 ) );

                // variables
                for ( std::size_t j = 0, dim = this->update_variable_.at( i ).size() - 1; j < dim; ++j )
                {
                    fprintf( csv, "%.15f,", this->update_variable_.at( i ).at( j ) );
                }
                fprintf( csv, "%.15f\n", this->update_variable_.at( i ).back() );
            }
            fclose( csv );
        }

        /*
         * 最良変数
         * 1試行のなかで最も評価値が良かった時の変数の値を示す。
         *
         * # 引数
         *   file_name :string 出力するファイル名。拡張子を含めること。
         */
        void TraitResult::tprint_variable( const std::string& file_name ) const
        {
            FILE* csv = fopen( file_name.c_str(), "w" );

            // ヘッダー印字
            fprintf( csv, "Variable[n],Value\n" );

            // 係数値印字
            for ( std::size_t i = 0, dim = this->variable_.size() - 1; i < dim; i++ )
            {
                fprintf( csv, "Variable[%lu],%.15f\n", static_cast< long unsigned int >( i ), this->variable_.at( i ) );
            }
            fprintf( csv, "Variable[%lu],%.15f\n", static_cast< long unsigned int >( this->variable_.size() ), this->variable_.back() );

            fclose( csv );
        }

        /*
         * 開始時の最良変数
         * 最適化開始時で最も評価値が良かった時の変数の値を示す。
         *
         * # 引数
         *   file_name :string 出力するファイル名。拡張子を含めること。
         */
        void TraitResult::tprint_init_variable( const std::string& file_name ) const
        {
            FILE* csv = fopen( file_name.c_str(), "w" );

            // header 印字
            fprintf( csv, "No," );
            for ( std::size_t i = 0, len = this->init_variable_.at( 0 ).size(); i < len; i++ )
            {
                fprintf( csv, "Variable[%lu],", static_cast< long unsigned int >( i ) );
            }
            fprintf( csv, "\n" );

            // 係数値印字
            for ( std::size_t i = 0, len = this->init_variable_.size(); i < len; ++i )
            {
                // No
                fprintf( csv, "%lu,", static_cast< long unsigned int >( i + 1 ) );

                // variables
                for ( std::size_t j = 0, dim = this->init_variable_.at( i ).size() - 1; j < dim; ++j )
                {
                    fprintf( csv, "%.15f,", this->init_variable_.at( i ).at( j ) );
                }
                fprintf( csv, "%.15f\n", this->init_variable_.at( i ).back() );
            }

            fclose( csv );
        }
    }    // namespace result
}    // namespace mtst_common
