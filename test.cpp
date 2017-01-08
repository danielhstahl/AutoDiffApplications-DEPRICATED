#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "BlackScholes.h"
#include "AutoDiff.h"
#include <cmath>


TEST_CASE("Test Delta", "[BS]"){
    const auto S0=AutoDiff<double>(50, 1);
    const auto r=.05;
    const auto t=1.0;
    const auto discount=exp(-r*t);
    const auto k=50;
    const auto sigma=.3*sqrt(t);

    double s=sqrt(2.0);
    auto d1=log(50.0/(discount*k))/(sigma)+sigma*.5;
    REQUIRE(BSCall(S0, discount, k, sigma).getDual()==Approx(.5*erf(d1/s)+.5));
}