#ifndef __BLACKSCHOLES_H_INCLUDED__
#define __BLACKSCHOLES_H_INCLUDED__
#include <cmath>
#include "AutoDiff.h"
#include <assert.h>

auto BSCall(
    const auto&,/*underlying*/
    const auto&, /*discount factor (in pure BS, discount=e^{-rt}) */
    const auto&, /*strike*/
    const auto&, /*volatility*/
    const auto& /*maturity*/
);
auto BSPut(
    const auto&,/*underlying*/
    const auto&, /*discount factor (in pure BS, discount=e^{-rt}) */
    const auto&, /*strike*/
    const auto&, /*volatility*/
    const auto& /*maturity*/
);
#include "BlackScholes.hpp"

#endif
