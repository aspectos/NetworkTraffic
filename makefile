# Copyright 2021, Pejman Taslimi
#  You may distribute under the terms of either the GNU General Public
#  License or the Artistic License, as specified in the MYLICENSE file.
# 
#  FILENAME     :	
#  DESCRIPTION  :	
#  
#  Author       : Mani Tasl
#  Date         : Jun 2021.
# 
#
LIBS=-lpcap -lpthread -LFTXUI/lib -lcomponent -lscreen -ldom -L.
# LIBS=-lboost_regex -lpcap 
LIBCAP=-lcaptureFuncs
INC=-IFTXUI/include -I.
CXXFLAG=-std=c++17
all: toggle capture truncate libcaptureFuncs.a captureFuncs.o captureFuncsMain capui
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
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(INC) $(LIBS) $(LIBCAP)
captureFuncsMain: captureFuncsMain.cpp
	$(CXX) $(CXXFLAG) -o $@ $@.cpp $(INC) $(LIBS) $(LIBCAP)
captureFuncs: captureFuncs.cpp
	$(CXX) $(CXXFLAG) -c $@ $@.cpp $(INC) $(LIBS)
libcaptureFuncs.a: captureFuncs.o
	ar rcs $@ captureFuncs.o
clean:
	rm capture truncate toggle capui libcaptureFuncs.a captureFuncsMain captureFuncs.o
