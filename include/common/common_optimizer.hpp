/*
 * common_optimizer.hpp
 *
 *  Created on: 2021/12/12
 *      Author: matsu
 */

#ifndef INCLUDE_COMMON_OPTIMIZER_HPP_
#define INCLUDE_COMMON_OPTIMIZER_HPP_

#include <functional>
#include <vector>

namespace mtst_common
{
    namespace optimizer
    {
        template< class R >
        struct TraitOptimizer
        {
        protected:


        public:

            TraitOptimizer() {}
            virtual ~TraitOptimizer() {}

            virtual R optimize( std::size_t, std::function< double( std::vector< double >& ) > ) = 0;

            virtual void initialize() = 0;
            virtual void pre_act() = 0;
            virtual void post_act() = 0;
            virtual void finalize() = 0;
        };
    }    // namespace optimizer
}    // namespace mtst_common


#endif /* INCLUDE_COMMON_OPTIMIZER_HPP_ */
