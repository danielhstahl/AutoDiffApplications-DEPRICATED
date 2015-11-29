#include "BlackScholes.h"
auto BSCall(auto &S0, auto &k, auto &discount, auto &sigma, auto &t){
  assert(t>0);
  double s=sqrt(2.0);
  auto sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5+.5*erf(d1/s))-k*discount*(.5+.5*(erf((d1-sigma*sqrtT)/s)));
}
auto BSPut(auto &S0, auto &k, auto &discount, auto &sigma, auto &t){
  assert(t>0);
  double s=sqrt(2.0);
  auto sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5*erf(d1/s)-.5)+k*discount*(.5-.5*(erf((d1-sigma*sqrtT)/s)));
}
