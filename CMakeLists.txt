cmake_minimum_required(VERSION 3.2)
project(vapors LANGUAGES CXX)

# Version define
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

# Set to c++11
set(CMAKE_CXX_STANDARD 11)

# Include the include/ directory
include_directories(include)

# Include thirdparty headers
include_directories(thirdparty/yaml-cpp/include)

# Build thirdparty libraries
add_subdirectory(thirdparty/yaml-cpp EXCLUDE_FROM_ALL)

set(SOURCE_FILES
        src/main.cc
        src/log.cc
        src/template.cc
        src/sockets/server.cc
        src/sockets/session.cc)

# Define main target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Append libraries to linker
target_link_libraries(${PROJECT_NAME} pthread yaml-cpp)

# Defines install prefix & install target
set(CMAKE_INSTALL_PREFIX /)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
install(DIRECTORY extra/ DESTINATION etc/${PROJECT_NAME})