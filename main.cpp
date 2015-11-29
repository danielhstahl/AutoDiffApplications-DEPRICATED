#define _USE_MATH_DEFINES
#include <iostream>
#include "BlackScholes.h"
#include "HullWhite.h"
#include <unordered_map>

int main(){
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
