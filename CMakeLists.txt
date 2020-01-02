cmake_minimum_required(VERSION 3.6)
project(ValiantUC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE String
            "Choose the type how to build, options are: None, Release, Debug, RelWithDebInfo, MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cmake-build-debug)
endif(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)

file(GLOB SOURCES src/Simple/*.cpp src/Circuit/*.cpp src/Dag/*.cpp src/Eu/*.cpp src/Gate/*.cpp src/SHDL/*.cpp)
file(GLOB HEADERS src/Simple/*.h src/Circuit/*.h src/Dag/*.h src/Eu/*.h src/Gate/*.h src/SHDL/*.h)
add_library(SRC_LIB ${SOURCES} ${HEADERS} src/UC.cpp src/UC.h src/Universal_Circuit.cpp)


add_executable(UC src/Universal_Circuit.cpp)
target_link_libraries(UC SRC_LIB)


add_executable(bristol src/bristol_to_SHDL.cpp src/config.h)
