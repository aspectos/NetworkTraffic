LIBS=-lboost_regex -lpcap
CXXFLAG=-std=c++17
code01: code01.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
code02: code02.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)

clean:
	rm code01
