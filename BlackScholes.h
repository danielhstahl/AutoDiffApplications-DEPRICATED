#ifndef __BlackScholes_H_INCLUDED__
#define __BlackScholes_H_INCLUDED__
#include <cmath>
#include "AutoDiff.h"

//template<typename Underlying, typename Strike, typename Discount, typename BSSigma, typename OptionMaturity> /*every element is a candidate for sensitivities. */
auto BSCall(auto &S0, auto &k, auto &discount, auto &sigma, auto &t){
  /*if(t==0){
    if(S0>k){
      return S0-k;
    }
    else{
      return 0; //
    }
  }*/
  double s=sqrt(2.0);
  auto sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5+.5*erf(d1/s))-k*discount*(.5+.5*(erf((d1-sigma*sqrtT)/s)));
}
//template<typename Underlying, typename Strike, typename Discount, typename BSSigma, typename OptionMaturity> /*every element is a candidate for sensitivities.  */
auto BSPut(auto &S0, auto &k, auto &discount, auto &sigma, auto &t){
  /*if(t==0){
    if(S0<k){
      return k-S0;
    }
    else{
      return 0; //
    }
  }*/
  double s=sqrt(2.0);
  auto sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5*erf(d1/s)-.5)+k*discount*(.5-.5*(erf((d1-sigma*sqrtT)/s)));
}

#endif
