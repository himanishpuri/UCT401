# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = operating_system_lab

# Source files
SRCS = main.cpp src/OperatingSystemLab.cpp util/logger.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET) clean_objs

# Link the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean all generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Clean only object files
clean_objs:
	rm -f $(OBJS) */*.o

.PHONY: all clean clean_objs
