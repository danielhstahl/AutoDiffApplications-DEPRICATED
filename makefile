LDFLAGS=-L../NewtonOptimization -lNewton -L../MiscellaniousUtilities -lDate  -L../eigen -L../BinomialTree -lTree -L../AutoDiff -lAutoDiff
INCLUDES=-I../NewtonOptimization -I../MiscellaniousUtilities -I../eigen -I../BinomialTree -I../rapidjson -I../AutoDiff

OptionPricing: main.o
	g++ -std=c++14 -O3  -w -fPIC main.o  $(LDFLAGS) $(INCLUDES) -o OptionPricing -fopenmp

main.o: main.cpp BlackScholes.h BlackScholes.hpp HullWhite.h HullWhite.hpp
	g++ -std=c++14 -O3  -w -c -fPIC main.cpp $(LDFLAGS) $(INCLUDES) -fopenmp

clean:
	     -rm *.o OptionPricing
