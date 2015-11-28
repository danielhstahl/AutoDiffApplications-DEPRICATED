#ifndef __BlackScholes_H_INCLUDED__
#define __BlackScholes_H_INCLUDED__
#include <cmath>
#include "AutoDiff.h"

template<typename Underlying, typename Strike, typename Discount, typename BSSigma, typename OptionMaturity> /*every element is a candidate for sensitivities. */
auto BSCall(Underlying &S0, Strike &k, Discount &discount, BSSigma &sigma, OptionMaturity &t){
  /*if(t==0){
    if(S0>k){
      return S0-k;
    }
    else{
      return S0-S0; //
    }
  }*/
  double s=sqrt(2.0);
  OptionMaturity sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5+.5*erf(d1/s))-k*discount*(.5+.5*(erf((d1-sigma*sqrtT)/s)));
}
template<typename Underlying, typename Strike, typename Discount, typename BSSigma, typename OptionMaturity> /*every element is a candidate for sensitivities.  */
auto BSPut(Underlying S0, Strike k, Discount discount, BSSigma sigma, OptionMaturity t){
  double s=sqrt(2.0);
  OptionMaturity sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5*erf(d1/s)-.5)+k*discount*(.5-.5*(erf((d1-sigma*sqrtT)/s)));
}

#endif
