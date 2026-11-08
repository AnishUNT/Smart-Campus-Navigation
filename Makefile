# Makefile for the Smart Campus Navigation and Facility Analytics System
# Build:  make        Run:  ./campus [data-directory]
CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Iinclude
TARGET   = campus
SRCS     = $(wildcard src/*.cpp)
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
