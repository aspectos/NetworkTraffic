LIBS=-lboost_regex -lpcap -lpthread -LFTXUI/lib -lcomponent -lscreen -ldom
# LIBS=-lboost_regex -lpcap 
INC=-IFTXUI/include
CXXFLAG=-std=c++17
all: capui capture truncate
code01: code01.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
code02: code02.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
capture: capture.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
truncate: truncate.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(LIBS)
toggle: toggle.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(INC) $(LIBS)
capui: capui.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(INC) $(LIBS)
captureFuncs: captureFuncs.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(INC) $(LIBS)
clean:
	rm code01 code02 capture truncate toggle
