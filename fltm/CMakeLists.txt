cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC -fopenmp -mtune=corei7-avx") 

add_subdirectory(test)
add_subdirectory(main)
