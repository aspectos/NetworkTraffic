LIBS=-lboost_regex -lpcap
CXXFLAG=-std=c++17
code01: code01.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
code02: code02.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
capture: capture.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
truncate: truncate.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
all: code01 code02 capture truncate
clean:
	rm code01 code02 capture truncate
