cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC -pedantic -fopenmp -mtune=corei7-avx") 

set(Boost_INCLUDE_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/include/)
set(Boost_LIBRARY_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/lib/)

set( ROOT_PROJECT ../ )



add_subdirectory(test)
add_subdirectory(main)
#add_subdirectory(autok)
