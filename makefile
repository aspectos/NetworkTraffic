LIBS=-lboost_regex -lpcap -lpthread -LFTXUI/lib -lcomponent -lscreen -ldom -L.
# LIBS=-lboost_regex -lpcap 
INC=-IFTXUI/include -I.
CXXFLAG=-std=c++17
all: capui capture truncate libcaptureFuncs.a captureFuncs.o captureFuncsMain
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
captureFuncsMain: captureFuncsMain.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(INC) $(LIBS) -lcaptureFuncs
captureFuncs: captureFuncs.cpp
	$(CXX) $(CXXFLAG) -c $@ $@.cpp $(INC) $(LIBS)
libcaptureFuncs.a: captureFuncs.o
	ar rcs $@ captureFuncs.o
clean:
	rm code01 code02 capture truncate toggle libcaptureFuncs.a captureFuncsMain captureFuncs
