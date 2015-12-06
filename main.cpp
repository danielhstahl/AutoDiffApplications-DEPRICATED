#define _USE_MATH_DEFINES
#include <iostream>
#include "BlackScholes.h"
#include "HullWhite.h"
//#include "BondUtilities.h"
#include "YieldSpline.h"
#include <unordered_map>


int main(){

  int n=29;//number of test yields
  std::vector<SpotValue> testYield;
  Date currDate;
  for(int i=0; i<n; ++i){
    testYield.push_back(SpotValue(currDate+(i+1), (i+1)*.001));
  }
  YieldSpline yld(testYield);
  std::vector<double> couponTimes(5);
  couponTimes[0]=.5;
  couponTimes[1]=1;
  couponTimes[2]=1.5;
  couponTimes[3]=2;
  couponTimes[4]=2.5;
  //AutoDiff price=Bond_Price(AutoDiff(.03, 1), .4, .04, .3, 1, yld);
  //std::cout<<price.getStandard()<<", "<<price.getDual()<<std::endl;
  //std::cout<<"Expectation: "<<muR(.03, .4, .04, .3, 1, yld)<<std::endl;
  std::cout<<Coupon_Bond_Put(.03, .4, .04, 1, .3, 1, couponTimes, .04, yld)<<std::endl;
  std::cout<<Coupon_Bond_Call(.03, .4, .04, 1, .3, 1, couponTimes, .04, yld)<<std::endl;

  /*const auto& r_t,
  const auto& a,
  const auto& sigma,
  const auto& strike,
  const auto& t,
  const auto& T,
  const std::vector<auto>& couponTimes,
  const auto& couponRate,
  auto& yieldClass*/
  /*auto Bond_Price(
    auto& r_t,
    auto& a,
    auto& sigma,
    auto& t,
    auto& T,
    auto& yieldClass
  )*/



  std::unordered_map<std::string, AutoDiff> parameters;
  parameters.insert({"Underlying", AutoDiff(50, 0)});
  parameters.insert({"Strike", AutoDiff(50, 0)});
  parameters.insert({"Maturity", AutoDiff(1, 0)});
  parameters.insert({"Sigma", AutoDiff(.3, 0)});
  parameters.insert({"R", AutoDiff(.03, 0)});

  bool is_first_iteration = true;
  std::cout<<"The following are the parameters used in this demonstration:"<<std::endl;
  for(const auto & parameterPair : parameters) {
     std::cout << parameterPair.first << ": ";
     std::cout<<parameterPair.second.getStandard();
     std::cout << std::endl;
  }
  std::cout<<"Choose one of the parameters to find the price and derivative with respect to that parameter: ";
  std::string response;
  std::cin>>response;
  std::cout<<response<<std::endl;
  while(parameters.find(response)==parameters.end()){
    std::cout<<"Parameter doesn't exist!  Choose a valid parameter:"<<std::endl;
    std::cin>>response;
  }
  parameters.find(response)->second.setDual(1);
  AutoDiff discount=exp(-parameters.find("R")->second*parameters.find("Maturity")->second);
  AutoDiff Call=BSCall(parameters.find("Underlying")->second, discount, parameters.find("Strike")->second, parameters.find("Sigma")->second, parameters.find("Maturity")->second);
  AutoDiff Put=BSPut(parameters.find("Underlying")->second, discount, parameters.find("Strike")->second, parameters.find("Sigma")->second, parameters.find("Maturity")->second);
  std::cout<<"Call Price: "<<Call.getStandard()<<" Partial Derivative with respect to "<<response<<": "<<Call.getDual()<<std::endl;
  std::cout<<"Put Price: "<<Put.getStandard()<<" Partial Derivative with respect to "<<response<<": "<<Put.getDual()<<std::endl;
}
