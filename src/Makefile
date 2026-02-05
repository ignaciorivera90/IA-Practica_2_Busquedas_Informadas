CXX = g++
CXXFLAGS = -std=c++23 -Wall

SOURCES = $(wildcard *.cc)
OBJ = $(SOURCES:.cc=.o)

EXECUTABLE = p02_ia

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXECUTABLE)
