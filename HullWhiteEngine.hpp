HullWhiteEngine::HullWhiteEngine(){
    
}
HullWhiteEngine::HullWhiteEngine(Number& a_, Number& sigma_){
    a=a_;
    sigma=sigma_;
}
auto HullWhiteEngine::HullWhitePrice(
        AssetFeatures& asset, 
        auto& rate, 
        auto& futureTime,
        auto& asOfDate
    ){
    switch(asset.type){
        case BOND:
            return Bond_Price(rate, a, sigma, futureTime-asOfDate, asset.Maturity-asOfDate, yld);
        case EURODOLLARFUTURE:
            return EuroDollarFuture(rate, a, sigma, futureTime-asOfDate, asset.Maturity-asOfDate, asset.Tenor, yld);
        case COUPONBOND:
            std::vector<double> coupons;
            for(auto it=asset.Coupons.being(); it!=asset.Coupons.end();++it){
                coupons.push_back(it-asOfDate);
            }
            return Coupon_Bond_Price(rate, a, sigma, futureTime-asOfDate, coupons, asset.CouponRate, yld);
        case BONDCALL:
            return Bond_Call(rate, a, sigma, futureTime-asOfDate, asset.Maturity-asOfDate, asset.UnderlyingMaturity-asOfDate, asset.Strike, yld);
        case BONDPUT:
            return Bond_Put(rate, a, sigma, futureTime-asOfDate, asset.Maturity-asOfDate, asset.UnderlyingMaturity-asOfDate, asset.Strike, yld);
        case COUPONBONDCALL:
            std::vector<double> coupons;
            for(auto it=asset.Coupons.being(); it!=asset.Coupons.end();++it){
                coupons.push_back(it-asOfDate);
            }
            return Coupon_Bond_Call(rate, a, sigma, asset.Strike, futureTime-asOfDate,  coupons, asset.CouponRate, yld);
        case COUPONBONDPUT:
            std::vector<double> coupons;
            for(auto it=asset.Coupons.being(); it!=asset.Coupons.end();++it){
                coupons.push_back(it-asOfDate);
            }
            return Coupon_Bond_Put(rate, a, sigma, asset.Strike, futureTime-asOfDate,  coupons, asset.CouponRate, yld);
        case CAPLET:
            return Caplet(rate, a, sigma, futureTime-asOfDate, asset.Maturity-asOfDate, asset.Tenor, asset.Strike, yld);
        case SWAP:
            return Swap_Price(rate, a, sigma, futureTime-asOfDate, asset.Maturity-asOfDate, asset.Tenor, asset.SwapRate, yld);
        case SWAPTION:
            return Swaption(rate, a, sigma, asset.Strike, futureTime-asOfDate, asset.UnderlyingMaturity-asOfDate, asset.Maturity-asOfDate, yld);
            
            
    }
}

#define BONDCALL 3
#define BONDPUT 4
#define COUPONBONDCALL 5
#define COUPONBONDPUT 6
#define CAPLET 7
#define SWAP 8 
#define SWAPTION 9