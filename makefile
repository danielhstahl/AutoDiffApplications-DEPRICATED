INCLUDES= -I../AutoDiff 

test: test.o
	g++ -std=c++14 -O3  -w -fPIC test.o  $(LDFLAGS) $(INCLUDES) -o test -fopenmp

test.o: test.cpp BlackScholes.h 
	g++ -std=c++14 -O3  -w -c -fPIC test.cpp $(LDFLAGS) $(INCLUDES) -fopenmp
clean:
	-rm *.o test
