CXX = clang++
CXX_FLAGS = -std=c++2a -g -Og -Wall -I.

TARGETS := $(patsubst %.cpp, src/%.cpp, main.cpp)
EXCLUDE := $(patsubst %.cpp, src/%.cpp, MDDChart_Tests.cpp GreedyMDDSolver_Tests.cpp LocalSearchMDDSolver_Tests.cpp)
SRCS = $(filter-out $(TARGETS) $(EXCLUDE), $(wildcard src/*.cpp))
OBJS = $(patsubst src/%.cpp, obj/%.o, $(SRCS))
EXE := $(patsubst src/%.cpp, %, $(TARGETS))

.PHONY: all configure clean

all: configure $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXX_FLAGS) $^ src/$@.cpp -o bin/$@_exe

obj/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

configure:
	mkdir -p obj
	mkdir -p bin

clean:
	$(RM) -rf obj/*.o bin/*
