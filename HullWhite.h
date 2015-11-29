#ifndef __HULLWHITE_H_INCLUDED__
#define __HULLWHITE_H_INCLUDED__
#include <cmath>
#include "AutoDiff.h"
#include <algorithm>
#include <assert.h>
#include <vector>
#include "Newton.h"
#include "HullWhite.hpp"
#include "BlackScholes.h" //for option pricing
/*Note: the fundamental times here are (0, t, T, TM).  0 is current time (and is reflective of the current yield curve), t is some future time that we may want to price options at given the underlying at that time, T is an "initial" maturity and TM a "Final" maturity.  While it is natural to think of (0<t<T<TM), I only require 0<t and 0<T<TM. Note that ALL TIMES ARE WITH RESPECT TO 0!  */

auto T_Forward_Bond_Volatility( /*This is the volatility of a forward bond B(t, TM)/(B(t, T)), TM>T and can be used for zero coupon bond pricing using BS formula.  TM may equal T+delta, but must be manually added before calling this function*/
  auto&,/*Speed of mean reversion ("a")*/
  auto&, /*interest rate volatility ("sigma") */
  auto&, /*future time t */
  auto&, /*Initial Bond maturity T*/
  auto& /*Final Bond maturity TM*/
);
auto A( /*A(t, T) from the Hull White PDE*/
  auto&,/*a*/
  auto&,/*t*/
  auto& /*T */
);
auto A( /*A(T-t) from the Hull White PDE*/
  auto&,/*a*/
  auto&/*T-t*/
);
auto C( /*C(t, T) from the Hull White PDE...this may require taking a reference to a yield class!*/
  auto&, /*a */
  auto&, /*a */
  auto&,/*t*/
  auto&, /*T */
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto muR(/*the expected value of r_t under risk neutral measure: E[r_T|t]*/
  auto&, /*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*interst rate volatility*/
  auto&, /*future time*/
  auto&, /*expectation horizon*/
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto varianceR(/*the variance of r_t under risk neutral measure */
  auto&, /*speed of mean reversion*/
  auto&, /*interst rate volatility*/
  auto&, /*future time*/
  auto& /*expectation horizon*/
);
auto Bond_Price(/*The zero coupon bond price under Hull White*/
  auto& r_t, /*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*future time*/
  auto&, /*Bond expiration*/
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Bond_Price(/*The zero coupon bond price under Hull White for t=0*/
  auto&, /*Bond expiration*/
  auto&/*This is a yield class passed here...this should include member functions "Forward(t)" and "Yield(t)" and these should be the instantanoues forward rate and the continuously compounded zero coupon (yield*t) at time 0*/
);
auto Coupon_Bond_Price(/*The coupon bond price under Hull White*/
  auto&, /*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*future time*/
  std::vector<auto>&, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  auto&, /*coupon rate*/
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Coupon_Bond_Price(/*The coupon bond price under Hull White at time 0*/
  std::vector<auto>& , /*these are coupon times FROM 0!  these should be in order though not required*/
  auto&, /*coupon rate*/
  auto& /*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Bond_Call(/*The price of a call option on zero coupon bond under Hull White*/
  auto&, /*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*future time*/
  auto&, /*option maturity*/
  auto&, /*bond maturity*/
  auto&, /*strike*/
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Bond_Call(/*The price of a call option on zero coupon bond under Hull White at t=0*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*option maturity*/
  auto&, /*bond maturity*/
  auto&, /*strike*/
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Coupon_Bond_Call(/*The price of a call option on coupon bond under Hull White...uses jamshidian's trick*/
  auto&, /*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*strike*/
  auto&, /*future time*/
  std::vector<auto>&, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  auto&, /* coupon rate */
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Bond_Put(/*The price of a Put option on zero coupon bond under Hull White*/
  auto&, /*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*future time*/
  auto&, /*option maturity*/
  auto&, /*bond maturity*/
  auto&, /*strike*/
  auto& /*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Bond_Put(/*The price of a Put option on zero coupon bond under Hull White at t=0*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*option maturity*/
  auto&, /*bond maturity*/
  auto&, /*strike*/
  auto& /*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Coupon_Bond_Put(/*The price of a put option on coupon bond under Hull White...uses jamshidian's trick*/
  auto&,/*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*strike*/
  auto&, /*future time*/
  std::vector<auto>&, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  auto&, /*coupon rate*/
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Caplet(/*price of a caplet on simple bond yield*/
  auto&,/*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility*/
  auto&, /*future time*/
  auto&, /*option maturity*/
  auto&, /*tenor of the simple yield*/
  auto&, /*strike*/
  auto& /*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto Caplet(/*price of a caplet on simple bond yield at time t=0*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility */
  auto&, /*option maturity*/
  auto&, /*tenor of the simple yield*/
  auto&, /*strike*/
  auto& /*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);
auto EuroDollarFuture(
  auto&,/*r_t*/
  auto&, /*speed of mean reversion*/
  auto&, /*volatility */
  auto&, /*future time*/
  auto&, /*maturity*/
  auto&, /*tenor of the simple yield*/
  auto&/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
);



#endif
