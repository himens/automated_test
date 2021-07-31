# Exec
EXE := test.exe

# Compiler
CXX = g++

# Paths
SRC := src
INC := include

# Compiler flags
CXXFLAGS := -Wall -fPIC -O3 -std=c++11 # C++11

# Src, obj, inc
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(EXE)

# Make exec
$(EXE): $(OBJS) 
	$(CXX) $(CXXFLAGS) $^ -o $@ 

# Make objs
%.o: %.cpp $(INC)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC)

# Clean
clean:
	-rm $(OBJS)
	-rm $(EXE)
