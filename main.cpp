#define _USE_MATH_DEFINES
#include <iostream>
#include "BlackScholes.h"

#include "HullWhiteEngine.h"
#include <string>
#include "YieldIO.h"
#include "RealWorldMeasure.h"
#include "CurveFeatures.h"
#include "YieldSpline.h"
#include <unordered_map>
#include "HandlePath.h" //for creating sample paths
#include "MC.h" //monte carlo
#include "SimulateNorm.h"
#include "document.h" //rapidjson
#include "writer.h" //rapidjson
#include "stringbuffer.h" //rapidjson

int main(){

  /*int n=29;//number of test yields
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
  couponTimes[4]=2.5;*/

  //std::cout<<Coupon_Bond_Put(.03, .4, .04, 1, .3, 1, couponTimes, .04, yld)<<std::endl;
  //std::cout<<Coupon_Bond_Call(.03, .4, .04, 1, .3, 1, couponTimes, .04, yld)<<std::endl;
    /*auto swapRate=Swap_Rate(.03, .4, .04, 1, 5, .25, yld);
  std::cout<<swapRate<<std::endl;
    std::cout<<Swap_Price(.03, .4, .04, 1, 5, .25, swapRate, yld)<<std::endl;*/
    
    
  //std::cout<<Swaption(.03, .4, .04, .04, .3, 5, 1, .25, yld)<<std::endl;
    Date currDate; 
    YieldSpline yld;
    double b;//long run average
    populateYieldFromExternalSource(currDate, yld, b);//this will wait from input from external source
    yld.getSpotCurve();//send data to node
    yld.getForwardCurve(); //send data ot node
    
    HullWhiteEngine<double> HW;
    //HW.setYield(&yld);
    double r0=yld.getShortRate(); //note we can change this here to an AutoDiff if we want sensitivities
    //HW.setShortRate(r0);
    SimulateNorm rNorm;
    
    //std::cout<<"r0: "<<r0<<std::endl;
    
    
    
    MC<double> monteC;
    //std::unordered_map<std::string, double> parameters;
    
    std::string parameters;
    for (parameters; std::getline(std::cin, parameters);) {
        break;
    }
    //std::cout<<parameters<<std::endl;
    rapidjson::Document parms;

    parms.Parse(parameters.c_str());//yield data
    parameters.clear();
    
    

    std::vector<AssetFeatures> portfolio; 
    AssetFeatures asset;
    //parse the string that came in
    currDate.setScale("year");
    if(parms.FindMember("T")!=parms.MemberEnd()){
        asset.Maturity=currDate+parms["T"].GetDouble();
    }
    if(parms.FindMember("k")!=parms.MemberEnd()){
        asset.Strike=parms["k"].GetDouble();
    }
    if(parms.FindMember("delta")!=parms.MemberEnd()){
        asset.Tenor=parms["delta"].GetDouble();
    }
    if(parms.FindMember("Tm")!=parms.MemberEnd()){
        asset.UnderlyingMaturity=currDate+parms["Tm"].GetDouble();
    }
    asset.type=parms["asset"].GetInt();
    double a=parms["a"].GetDouble(); //can be made autodiff too
    double sigma=parms["sigma"].GetDouble(); //can be made autodiff too
    HW.setSigma(sigma);
    HW.setReversion(a);
    
    
    
    currDate.setScale("day");
    Date PortfolioMaturity;
    //std::cout<<parms["t"].GetInt()<<std::endl;
    if(parms.FindMember("t")!=parms.MemberEnd()){
        PortfolioMaturity=currDate+parms["t"].GetInt();
    }
    int m=0;  
    if(parms.FindMember("n")!=parms.MemberEnd()){
        m=parms["n"].GetInt();
    }
    monteC.setM(m);
    portfolio.push_back(asset);
    std::vector<Date> path=getUniquePath(portfolio, PortfolioMaturity); 
     
    
    //std::cout<<Bond_Price(r0, a, sigma, PortfolioMaturity-currDate, asset.Maturity-currDate, yld)<<std::endl;
    
   // std::cout<<path[0]<<std::endl;
    monteC.simulateDistribution([&](){
        return executePortfolio(portfolio, currDate, 
            [&](const auto& currVal, const auto& time){
                double vl=rNorm.getNorm();
                return generateVasicek(currVal, time, a, b, sigma, vl);
            },
            r0,  
            path,  
            [&](AssetFeatures& asset, auto& rate, Date& maturity,   Date& asOfDate){
                return HW.HullWhitePrice(asset, rate, maturity, asOfDate, yld);
            }
        );
    });   

    
  /*std::unordered_map<std::string, AutoDiff> parameters;
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
  std::cout<<"Put Price: "<<Put.getStandard()<<" Partial Derivative with respect to "<<response<<": "<<Put.getDual()<<std::endl;*/
}
