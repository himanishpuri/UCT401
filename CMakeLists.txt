cmake_minimum_required(VERSION 3.10)
project(OperatingSystemLab LANGUAGES CXX)

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Source files
set(SRCS
    main.cpp
    src/OperatingSystemLab.cpp
    util/logger.cpp
)

# Define the executable target
add_executable(operating_system_lab ${SRCS})

# Organize files into logical groups (optional, improves project structure in IDEs)
source_group("src" FILES src/OperatingSystemLab.cpp)
source_group("util" FILES util/logger.cpp)

# Define a clean target (CMake has a built-in clean step, but this is for convenience)
add_custom_target(clean_objs
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/CMakeFiles/operating_system_lab.dir/*.o
    COMMENT "Cleaning object files"
)
