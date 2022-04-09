CXX = clang++
CXX_FLAGS = -std=c++2a -g -Og -Wall -I.

TARGETS := MDDChart_Tests.cpp GreedyMDDSolver_Tests.cpp 
SRCS = $(filter-out $(TARGETS), $(wildcard *.cpp))
OBJS = $(patsubst %.cpp, obj/%.o, $(SRCS))
EXE := $(patsubst %.cpp, %, $(TARGETS))

.PHONY: all configure clean

all: configure $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXX_FLAGS) $^ $@.cpp -o $@_exe

obj/%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

configure:
	mkdir -p obj

clean:
	$(RM) -rf obj/*.o *_exe *.dSYM
