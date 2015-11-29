#ifndef __BLACKSCHOLES_H_INCLUDED__
#define __BLACKSCHOLES_H_INCLUDED__
#include <cmath>
#include "AutoDiff.h"
#include <assert.h>  
#include "BlackScholes.hpp"

auto BSCall(auto&,/*underlying*/
            auto&, /*discount factor (in pure BS, discount=e^{-rt}) */
            auto&, /*strike*/
            auto&, /*volatility*/
            auto& /*maturity*/
);
auto BSPut(auto&,/*underlying*/
           auto&, /*discount factor (in pure BS, discount=e^{-rt}) */
           auto&, /*strike*/
           auto&, /*volatility*/
           auto& /*maturity*/
);


#endif
