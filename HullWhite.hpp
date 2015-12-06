auto A( /*A(t, T) from the Hull White PDE*/
  const auto& a,/*a*/
  const auto& t,/*t*/
  const auto& T /*T */
){
  return A(a, T-t);
}
auto A( /*A(T-t) from the Hull White PDE*/
  const auto& a,/*a*/
  const auto& tDiff/*T-t*/
){
  return (1.0-exp(-a*tDiff))/a;
}
auto C( /*C(t, T) from the Hull White PDE...this may require taking a reference to a yield class!*/
  const auto& a, /*a */
  const auto& sigma, /*a */
  const auto& t,/*t*/
  const auto& T, /*T */
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  auto sqr=A(a, t)-A(a, T);
  sqr=sqr*sqr;
  return yieldClass.Yield(t)-yieldClass.Yield(T)-yieldClass.Forward(t)*A(a, t, T)-sigma*sigma*sqr*(exp(2*a*t)-1.0)/(4*a*a*a);
}
auto T_Forward_Bond_Volatility( /*This is the volatility of a forward bond B(t, TM)/(B(t, T)), TM>T and can be used for zero coupon bond pricing using BS formula.  TM may equal T+delta, but must be manually added before calling this function*/
  const auto& a,/*Speed of mean reversion ("a")*/
  const auto& sigma, /*interest rate volatility ("sigma") */
  const auto& t, /*future time t */
  const auto& T, /*Initial Bond maturity T*/
  const auto& TM/*Final Bond maturity TM*/
){
  return sigma*sqrt((exp(2*a*T)-exp(2*a*t))/(2*a))*(A(a, TM)-A(a, T));
}
auto muR(/*the expected value of r_t under risk neutral measure: E[r_T|t]*/
  const auto& r_t,
  const auto& a, /*speed of mean reversion*/
  const auto& sigma, /*interst rate volatility*/
  const auto& t, /*future time*/
  const auto& T, /*expectation horizon*/
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  auto expT=exp(-a*(T-t));
  return expT*r_t+yieldClass.Forward(T)-expT*yieldClass.Yield(t)+sigma*sigma*(1-expT+exp(-2*a*T)-exp(-a*(T+t)))/(2*a*a);
}
auto varianceR(/*the variance of r_t under risk neutral measure */
  const auto& a, /*speed of mean reversion*/
  const auto& sigma, /*interst rate volatility*/
  const auto& t, /*future time*/
  const auto& T /*expectation horizon*/
){
  return sigma*sigma*(1-exp(-2*a*(T-t)))/(2*a);
}
auto Bond_Price(/*The zero coupon bond price under Hull White*/
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  const auto& T, /*Bond expiration*/
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  return exp(-r_t*A(a, t, T)+C(a, sigma, t, T, yieldClass));
}
auto Bond_Price(/*The zero coupon bond price under Hull White for t=0*/
  const auto& T, /*Bond expiration*/
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward(t)" and "Yield(t)" and these should be the instantanoues forward rate and the continuously compounded zero coupon (yield*t) at time 0*/
){
  return exp(-yieldClass.Yield(T));
}
auto Coupon_Bond_Price(/*The coupon bond price under Hull White*/
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  const auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  int n=couponTimes.size();
  std::sort(couponTimes.begin(), couponTimes.end());
  assert(couponTimes[n-1]>t);
  auto bondprice=Bond_Price(r_t, a, sigma, t, couponTimes[n-1], yieldClass)*(1+couponRate);
  for(int i=0; i<(n-1); ++i){
    if(couponTimes[i]>t){
      bondprice+=Bond_Price(r_t, a, sigma, t, couponTimes[i], yieldClass)*couponRate;
    }
  }
  return bondprice;
}
auto Coupon_Bond_Price(/*The coupon bond price under Hull White at time 0*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order*/
  const auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  int n=couponTimes.size();
  auto bondprice=Bond_Price(couponTimes[n-1], yieldClass)*(1+couponRate);
  for(int i=0; i<(n-1); i++){
    bondprice+=Bond_Price(couponTimes[i], yieldClass)*couponRate;
  }
  return bondprice;
}
auto Bond_Call(/*The price of a call option on zero coupon bond under Hull White*/
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  const auto& T, /*option maturity*/
  const auto& TM, /*bond maturity*/
  const auto& strike,
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
  const auto& a,
  const auto& sigma,
  const auto& T, /*option maturity*/
  const auto& TM, /*bond maturity*/
  const auto& strike,
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
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& strike,
  const auto& t, /*future time*/
  const auto& T, /*option maturity*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  const auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  Newton nt;
  int n=couponTimes.size();
  auto guess=r_t;
  nt.zeros([&](auto &r){
    return Coupon_Bond_Price(r, a, sigma, T, couponTimes, couponRate, yieldClass)-strike; //T is "future" time since bond is priced at opion maturity
  }, guess);
  auto retVal=(1+couponRate)*Bond_Call(r_t, a, sigma, t, T, couponTimes[n-1], Bond_Price(guess, a, sigma, T, couponTimes[n-1], yieldClass), yieldClass);
  for(int i=0; i<(n-1); i++){
    if(couponTimes[i]>T){
      retVal+=couponRate*Bond_Call(r_t, a, sigma, t, T, couponTimes[n-1], Bond_Price(guess, a, sigma, T, couponTimes[n-1], yieldClass), yieldClass);
    }
  }
  return retVal;
}
auto Bond_Put(/*The price of a Put option on zero coupon bond under Hull White*/
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  const auto& T, /*option maturity*/
  const auto& TM, /*bond maturity*/
  const auto& strike,
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
  const auto& a,
  const auto& sigma,
  const auto& T, /*option maturity*/
  const auto& TM, /*bond maturity*/
  const auto& strike,
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
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& strike,
  const auto& t, /*future time*/
  const auto& T, /*option maturity*/
  std::vector<auto>& couponTimes, /*these are coupon times FROM 0!  these should be in order but dont have to be*/
  const auto& couponRate,
  auto& yieldClass/*This is a yield class passed here...this should include member functions "Forward" and "Yield" and these should be the instantanoues forward rate and the continuously compounded zero coupon yield*/
){
  Newton nt;
  int n=couponTimes.size();
  auto guess=r_t;
  nt.zeros([&](auto &r){
    return Coupon_Bond_Price(r, a, sigma, T, couponTimes, couponRate, yieldClass)-strike; //T is "future" time since bond is priced at opion maturity
  }, guess);
  auto retVal=(1+couponRate)*Bond_Put(r_t, a, sigma, t, T, couponTimes[n-1], Bond_Price(guess, a, sigma, T, couponTimes[n-1], yieldClass), yieldClass);
  for(int i=0; i<(n-1); i++){
    if(couponTimes[i]>T){
      retVal+=couponRate*Bond_Put(r_t, a, sigma, t, T, couponTimes[n-1], Bond_Price(guess, a, sigma, T, couponTimes[n-1], yieldClass), yieldClass);
    }
  }
  return retVal;
}
auto Caplet(/*price of a caplet on simple bond yield*/
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  const auto& T, /*option maturity*/
  const auto& delta, /*tenor of the simple yield*/
  const auto& strike,
  auto& yieldClass
){
  return (strike*delta+1)*Bond_Put(r_t, a, sigma, t, T, T+delta, 1/(delta*strike+1), yieldClass);
}
auto Caplet(/*price of a caplet on simple bond yield at time t=0*/
  const auto& a,
  const auto& sigma,
  const auto& T, /*option maturity*/
  const auto& delta, /*tenor of the simple yield*/
  const auto& strike,
  auto& yieldClass
){
  return (strike*delta+1)*Bond_Put(a, sigma, T, T+delta, 1/(delta*strike+1), yieldClass);
}
auto EuroDollarFuture(
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  const auto& T, /*maturity*/
  const auto& delta, /*tenor of the simple yield*/
  auto& yieldClass
){
  auto aTTDelta=A(a, T, T+delta);
  return exp(aTTDelta*muR(r_t, a, sigma, t, T, yieldClass)+.5*aTTDelta*aTTDelta*varianceR(a, sigma, t, T)-C(a, sigma, t, T, yieldClass));
}
auto Swap_Rate(
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  const auto& T, /*swap maturity*/
  const auto& delta, /*tenor of the floating rate*/
  auto& yieldClass
){
  int numPayments=(T-t)/delta+1; //this should be an integer!  remember, T-t is the total swap length
  auto denominator=Bond_Price(r_t, a, sigma, t, T+delta, yieldClass); //final bond price
  auto num=1.0-denominator;
  for(int i=0; i<(numPayments-1); ++i){
    denominator+=Bond_Price(r_t, a, sigma, t, t+delta*(i+1), yieldClass);
  }
  return num/(delta*denominator);
}

auto Swaption(/*TODO!  most of this function!*/
  const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& t, /*future time*/
  const auto& T, /*swap maturity*/
  const auto& TM, /*option maturity*/
  const auto& delta, /*tenor of the floating rate*/
  const auto& strike,
  auto& yieldClass
){
  Newton nt;
  //int n=cashFlowFimes.size();
  //std::vector<double> c(n);
  //c[0]=k*(cashFlowFimes[0]-optMat);
  //for(int i=1; i<n; i++){
  //  c[i]=k*(cashFlowFimes[i]-cashFlowFimes[i-1]);
  //}
  //c[n-1]=c[n-1]+1;
  //double guess=r;
  nt.zeros([&](double r){
    double retVal=0;
    for(int i=0; i<n; i++){
      retVal+=c[i]*Bond_Price(r, optMat, cashFlowFimes[i]);
    }
    return retVal-1;
  },guess);
  double retVal=0;
  //std::cout<<"tfut: "<<tFut<<" optMat: "<<optMat<<std::endl;
//  std::cout<<cashFlows[0]<<std::endl;
  //std::cout<<Bond_Put(r, tFut, 1, 1, optMat)<<std::endl;
  for(int i=0; i<n; i++){
    /*if(isnan(c[i]*Bond_Put(r, tFut, Bond_Price(guess, optMat, cashFlows[i]), cashFlows[i], optMat))){
      std::cout<<"i: "<<i<<" tfut: "<<tFut<<" guess: "<<guess<<" optMat: "<<optMat<<"cashFlowi: "<<cashFlows[i]<<std::endl;
    }*/
    retVal+=c[i]*Bond_Put(r, tFut, Bond_Price(guess, optMat, cashFlowFimes[i]), cashFlowFimes[i], optMat);
    //std::cout<<"retVal: "<<retVal<<" i: "<<i<<std::endl;
  }
  //std::cout<<retVal<<std::endl;
  return retVal;
}
