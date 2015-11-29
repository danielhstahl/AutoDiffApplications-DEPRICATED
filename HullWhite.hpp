//#include "HullWhite.h"

auto A( /*A(t, T) from the Hull White PDE*/
  auto& a,/*a*/
  auto& t,/*t*/
  auto& T /*T */
){
  return A(a, T-t);
}
auto A( /*A(T-t) from the Hull White PDE*/
  auto& a,/*a*/
  auto& tDiff/*T-t*/
){
  return (1.0-exp(-a*tDiff))/a;
}
auto C( /*C(t, T) from the Hull White PDE...this may require taking a reference to a yield class!*/
  auto& a, /*a */
  auto& sigma, /*a */
  auto& t,/*t*/
  auto& T, /*T */
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  auto sqr=A(a, t)-A(A, T);
  sqr=sqr*sqr;
  return yieldClass.Yield(t)-yieldClass.Yield(T)-yieldClass.Forward(t)*A(a, t, T)-sigma*sigma*sqr*(exp(2*a*t)-1.0)/(4*a*a*a);
}
auto T_Forward_Bond_Volatility( /*This is the volatility of a forward bond B(t, TM)/(B(t, T)), TM>T and can be used for zero coupon bond pricing using BS formula.  TM may equal T+delta, but must be manually added before calling this function*/
  auto& a,/*Speed of mean reversion ("a")*/
  auto& sigma, /*interest rate volatility ("sigma") */
  auto& t, /*future time t */
  auto& T, /*Initial Bond maturity T*/
  auto& TM/*Final Bond maturity TM*/
){
  return sigma*sqrt((exp(2*a*T)-exp(2*a*t))/(2*a))*(A(a, TM)-A(a, T));
}
auto muR(/*the expected value of r_t under risk neutral measure: E[r_T|t]*/
  auto& r_t,
  auto& a, /*speed of mean reversion*/
  auto& sigma, /*interst rate volatility*/
  auto& t, /*future time*/
  auto& T, /*expectation horizon*/
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  auto expT=exp(-a*(T-t));
  return expT*r_t+yieldClass.Forward(T)-expT*yieldClass.Yield(t)+sigma*sigma*(1-expT+exp(-2*a*T)-exp(-a*(T+t)))/(2*a*a);
}
auto varianceR(/*the variance of r_t under risk neutral measure */
  auto& a, /*speed of mean reversion*/
  auto& sigma, /*interst rate volatility*/
  auto& t, /*future time*/
  auto& T /*expectation horizon*/
){
  return sigma*sigma*(1-exp(-2*a*(T-t)))/(2*a);
}
auto Bond_Price(/*The zero coupon bond price under Hull White*/
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& t, /*future time*/
  auto& T, /*Bond expiration*/
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  return exp(-r_t*A(a, t, T)+C(a, sigma, t, T, yieldClass));
}
auto Bond_Price(/*The zero coupon bond price under Hull White for t=0*/
  auto& T, /*Bond expiration*/
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward(t)" and "Yield(t)" and these should be the instantanoues forward rate and the continuously compounded zero coupon (yield*t) at time 0*/
){
  return exp(-yieldClass.Yield(T));
}
auto Coupon_Bond_Price(/*The coupon bond price under Hull White*/
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& t, /*future time*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  int n=couponTimes.size();
  std::sort(couponTimes.begin(), couponTimes.end());
  assert(couponTimes[n-1]>t);
  auto bondprice=Bond_Price(r_t, a, sigma, t, couponTimes[n-1], yieldClass);
  for(int i=0; i<n; i++){
    if(couponTimes[i]>t){
      bondprice+=Bond_Price(r_t, a, sigma, t, couponTimes[i], yieldClass)*couponRate;
    }
  }
  return bondprice;
}
auto Coupon_Bond_Price(/*The coupon bond price under Hull White at time 0*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order*/
  auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  int n=couponTimes.size();
  auto bondprice=Bond_Price(couponTimes[n-1], yieldClass);
  for(int i=0; i<n; i++){
    bondprice+=Bond_Price(couponTimes[i], yieldClass)*couponRate;
  }
  return bondprice;
}
auto Bond_Call(/*The price of a call option on zero coupon bond under Hull White*/
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& t, /*future time*/
  auto& T, /*option maturity*/
  auto& TM, /*bond maturity*/
  auto& strike,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  return BSCall(
    Bond_Price(r_t, a, sigma, t, TM, yieldClass), /*underlying*/
    Bond_Price(r_t, a, sigma, t, T, yieldClass), /*discount factor*/
    strike,
    T_Forward_Bond_Volatility(a, sigma, t, T, TM), /*volatility of underlying*/
    T-t /*maturity*/
  );
}
auto Bond_Call(/*The price of a call option on zero coupon bond under Hull White at t=0*/
  auto& a,
  auto& sigma,
  auto& T, /*option maturity*/
  auto& TM, /*bond maturity*/
  auto& strike,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  return BSCall(
    Bond_Price(TM, yieldClass), /*underlying*/
    Bond_Price(T, yieldClass), /*discount factor*/
    strike,
    T_Forward_Bond_Volatility(a, sigma, 0, T, TM), /*volatility of underlying*/
    T /*maturity*/
  );
}
auto Coupon_Bond_Call(/*The price of a call option on coupon bond under Hull White...uses jamshidian's trick*/
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& strike,
  auto& t, /*future time*/
  auto& T, /*option maturity*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  Newton nt;
  int n=couponTimes.size();
  auto guess=r_t;
  nt.zeros([&](auto &r){
    //double retVal=0;
    return Coupon_Bond_Price(r, a, sigma, t, couponTimes, yieldClass)-strike;

  }, guess);
  //couponTimes was passed by reference to Coupon_Bond_Price, so it will already be sorted
  auto retVal=Bond_Call(r_t, a, sigma, t, T, couponTimes[n-1], Bond_Price(guess, T, couponTimes[n-1]), yieldClass);
  for(int i=0; i<n; i++){
    if(couponTimes[i]>t){
      retVal+=couponRate*Bond_Call(r_t, a, sigma, t, T, couponTimes[i], Bond_Price(guess, T, couponTimes[i]), yieldClass);
    }
  }
  return retVal;
}
auto Bond_Put(/*The price of a Put option on zero coupon bond under Hull White*/
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& t, /*future time*/
  auto& T, /*option maturity*/
  auto& TM, /*bond maturity*/
  auto& strike,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  return BSPut(
    Bond_Price(r_t, a, sigma, t, TM, yieldClass), /*underlying*/
    Bond_Price(r_t, a, sigma, t, T, yieldClass), /*discount factor*/
    strike,
    T_Forward_Bond_Volatility(a, sigma, t, T, TM), /*volatility of underlying*/
    T-t /*maturity*/
  );
}
auto Bond_Put(/*The price of a Put option on zero coupon bond under Hull White at t=0*/
  auto& a,
  auto& sigma,
  auto& T, /*option maturity*/
  auto& TM, /*bond maturity*/
  auto& strike,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  return BSPut(
    Bond_Price(TM, yieldClass), /*underlying*/
    Bond_Price(T, yieldClass), /*discount factor*/
    strike,
    T_Forward_Bond_Volatility(a, sigma, 0, T, TM), /*volatility of underlying*/
    T /*maturity*/
  );
}
auto Coupon_Bond_Put(/*The price of a put option on coupon bond under Hull White...uses jamshidian's trick*/
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& strike,
  auto& t, /*future time*/
  auto& T, /*option maturity*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  Newton nt;
  int n=couponTimes.size();
  auto guess=r_t;
  nt.zeros([&](auto &r){
    //double retVal=0;
    return Coupon_Bond_Price(r, a, sigma, t, couponTimes, yieldClass)-strike;

  }, guess);
  //couponTimes was passed by reference to Coupon_Bond_Price, so it will already be sorted
  auto retVal=Bond_Call(r_t, a, sigma, t, T, couponTimes[n-1], Bond_Price(guess, T, couponTimes[n-1]), yieldClass);
  for(int i=0; i<n; i++){
    if(couponTimes[i]>t){
      retVal+=couponRate*Bond_Put(r_t, a, sigma, t, T, couponTimes[i], Bond_Price(guess, T, couponTimes[i]), yieldClass);
    }
  }
  return retVal;
}
auto Caplet(/*price of a caplet on simple bond yield*/
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& t, /*future time*/
  auto& T, /*option maturity*/
  auto& delta, /*tenor of the simple yield*/
  auto& strike,
  auto& yieldClass
){
  return (strike*delta+1)*Bond_Put(r_t, a, sigma, t, T, T+delta, 1/(delta*strike+1), yieldClass);
}
auto Caplet(/*price of a caplet on simple bond yield at time t=0*/
  auto& a,
  auto& sigma,
  auto& T, /*option maturity*/
  auto& delta, /*tenor of the simple yield*/
  auto& strike,
  auto& yieldClass
){
  return (strike*delta+1)*Bond_Put(a, sigma, T, T+delta, 1/(delta*strike+1), yieldClass);
}
auto EuroDollarFuture(
  auto& r_t,
  auto& a,
  auto& sigma,
  auto& t, /*future time*/
  auto& T, /*maturity*/
  auto& delta, /*tenor of the simple yield*/
  auto& yieldClass
){
  auto aTTDelta=A(a, T, T+delta);
  return exp(aTTDelta*muR(r_t, a, sigma, t, T, yieldClass)+.5*aTTDelta*aTTDelta*varianceR(a, sigma, t, T)-C(a, sigma, t, T, yieldClass));
}
