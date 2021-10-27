
/*
 * opt_helper.hpp
 *
 *  Created on: 2021/09/25
 *      Author: hommyo, chatblanc_ciel
 *
 * This cord is written by UTF-8
 */

#ifndef LIB_OPT_HELPER_HPP_
#define LIB_OPT_HELPER_HPP_

#define _USE_MATH_DEFINES

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <regex>
#include <functional>

using namespace std;

struct Result
{
protected:
    double value;                               //最良評価値
    vector<double> update_value;
    vector<double> variable;                    //最良評価値の時の変数
    vector<double> init_variable;              // 最適化開始時の最良評価値の変数
    vector<vector<double>> update_variable;
    double evals;                               //評価回数
    clock_t time;                               //1試行あたりの最適化の実行時間
    size_t iter;                                //更新回数
    
public:

    Result(){}

    Result& set_value(double input)
    {
        value = input;
        return *this; 
    }

    Result& set_update_value(vector<double> input)
    { 
        update_value = input;
        return *this; 
    }

    Result& set_variable(vector<double> input)
    { 
        variable = input;
        return *this; 
    }  

    Result& set_init_variable(vector<double> input)
    { 
        init_variable = input;
        return *this; 
    }  

    Result& set_update_variable(vector<vector<double>> input)
    { 
        update_variable = input;
        return *this; 
    }

    Result& set_evals(double input)
    { 
        evals = input;
        return *this; 
    }

    Result& set_iter(size_t input)
    { 
        iter = input;
        return *this; 
    }

    Result& set_time(clock_t input)
    { 
        time = input;
        return *this; 
    }

    Result& set_time(double input)
    { 
        time = (clock_t)input;
        return *this; 
    }

    double get_value() const
    { return value; }
    vector<double> get_update_value() const
    { return update_value; }
    vector<double> get_variable() const
    { return variable; }
    vector<double> get_init_variable() const
    { return init_variable; }
    vector<vector<double>> get_update_variable() const
    { return update_variable; }   
    double get_evals() const
    { return evals; }
    clock_t get_time() const
    { return time; }
    size_t get_iter() const
    { return iter; }

    void tprint_update_curve(const string&) const;
    void gprint_update_curve(const string&, const bool) const; 
    void tprint_update_variable(const string&) const;  
    void tprint_variable(const string&) const;
    void tprint_init_variable(const string&) const;

    //static function

    static void tprint_all_result(const vector<Result>& ,const string&);
    static void tprint_all_variables(const vector<Result>& ,const string&);
    
};

struct Statistics
{
protected:

    double ave;
    double var;
    double std_dev;
    double max;
    double min;
    Result max_result;
    Result min_result;
    double evals_ave;
    clock_t time_ave;

public:

    Statistics(const vector<Result>&);

    double average() const
    { return ave; }
    double variance() const
    { return var; }
    double std_deviation() const
    { return std_dev; }
    double maximum() const
    { return max; }
    double minimum() const
    { return min; }
    double evals_average() const
    { return evals_ave; }
    clock_t time_average() const
    { return time_ave; }
    const Result& get_max_result() const
    { return ref(max_result); }
    const Result& get_min_result() const
    { return ref(min_result); }

    //static function

    static void tprint_all_statistics(const vector<Statistics>& ,const string&); 

};

#endif /* LIB_OPT_HELPER_HPP_ */
