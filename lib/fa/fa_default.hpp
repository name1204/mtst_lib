/*
 * fa_default.hpp
 *
 *  Created on: 2021/10/24
 *      Author: shimokawa
 */

#ifndef FA_DEFAULT_HPP_
#define FA_DEFAULT_HPP_

#include <vector>

namespace fa_default
{
	
	struct FireFly
    {
    public:

        FireFly();
		virtual ~FireFly();

	    std::vector<double> pos;
	    double value;
	    std::vector<double> vel;

	    std::vector<double> best_pos;
	    double best_value;
    };


}



#endif /* FA_DEFAULT_HPP_ */
