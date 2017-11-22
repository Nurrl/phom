cmake_minimum_required(VERSION 3.2)
project(vapors VERSION 0.1 LANGUAGES CXX)

# Version

# Set to c++11
set(CMAKE_CXX_STANDARD 11)

# Include the include/ directory
include_directories(include)

set(SOURCE_FILES
        src/main.cc
        src/log.cc
        src/sockets/server.cc
        src/sockets/session.cc)

# Define main target
add_executable(vapors ${SOURCE_FILES})

# Append libraries to linker
target_link_libraries(vapors pthread)
