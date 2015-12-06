#include "BlackScholes.h"
auto BSCall(const auto &S0, const auto &k, const auto &discount, const auto &sigma, const auto &t){
  assert(t>0);
  double s=sqrt(2.0);
  auto sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5+.5*erf(d1/s))-k*discount*(.5+.5*(erf((d1-sigma*sqrtT)/s)));
}
auto BSPut(const auto &S0, const auto &k, const auto &discount, const auto &sigma, const auto &t){
  assert(t>0);
  double s=sqrt(2.0);
  auto sqrtT=sqrt(t);
  auto d1=log(S0/(discount*k))/(sigma*sqrtT)+sigma*sqrtT*.5;
  return S0*(.5*erf(d1/s)-.5)+k*discount*(.5-.5*(erf((d1-sigma*sqrtT)/s)));
}
