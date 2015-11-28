LDFLAGS=-L../NewtonOptimization -lNewton -L../MiscellaniousUtilities -lDate  -L../eigen -L../BinomialTree -lTree -L../AutoDiff -lAutoDiff
INCLUDES=-I../NewtonOptimization -I../MiscellaniousUtilities -I../eigen -I../BinomialTree -I../rapidjson -I../AutoDiff

marketRisk: main.o BlackScholes.o
	g++ -std=c++11 -O3  -w -fPIC main.o BlackScholes.o $(LDFLAGS) $(INCLUDES) -o marketRisk -fopenmp

main.o: main.cpp BlackScholes.h
	g++ -std=c++11 -O3  -w -c -fPIC main.cpp $(LDFLAGS) $(INCLUDES) -fopenmp

BlackScholes.o: BlackScholes.cpp
	g++ -std=c++11 -O3  -w -c -fPIC BlackScholes.cpp $(LDFLAGS) $(INCLUDES) -fopenmp

clean:
	     -rm *.o marketRisk
