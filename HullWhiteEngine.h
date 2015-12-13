#ifndef __HULLWHITEENGINE_H_INCLUDED__
#define __HULLWHITEENGINE_H_INCLUDED__
#include <cmath>
#include "HullWhite.h"
#include "CurveFeatures.h" 
#include "YieldSpline.h"

#define BOND 0
#define EURODOLLARFUTURE 1
#define COUPONBOND 2
#define BONDCALL 3
#define BONDPUT 4
#define COUPONBONDCALL 5
#define COUPONBONDPUT 6
#define CAPLET 7
#define SWAP 8 
#define SWAPTION 9


 /*struct AssetFeatures{
    Date Maturity;
    Date UnderlyingMaturity;
    double Strike;
    double Tenor;
    const int type; //types are defined where?
     
 };*/
template<typename Number> //autodiff or double
class HullWhiteEngine{
    private:
        YieldSpline *yld;//pointer to yield class
        Number a;
        Number sigma;
    public:
        HullWhiteEngine();    
        HullWhiteEngine(Number&, Number&);    
        auto HullWhitePrice(
            AssetFeatures& asset, 
            auto& rate, 
            auto& futureTime
        );
    {
        switch(asset.type){
                case BOND:
                    return Bond_Price(rate, )
                case EURODOLLARFUTURE:

                case COUPONBOND:

            }
    }
};






#endif