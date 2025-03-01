CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = banker_algorithm__safety_algo
SRCS = banker_algorithm__safety_algo.cpp util/logger.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET) clean_objs

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

clean_objs:
	rm -f $(OBJS) */*.o

.PHONY: all clean clean_objs