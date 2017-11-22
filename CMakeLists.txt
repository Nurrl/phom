cmake_minimum_required(VERSION 3.2)
project(phom VERSION 0.1 LANGUAGES CXX)

# Version

# Set to c++11
set(CMAKE_CXX_STANDARD 11)

# Include the include/ directory
include_directories(include)

set(SOURCE_FILES
        src/main.cc)

# Define main target
add_executable(phom ${SOURCE_FILES})
