cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC -pedantic -fopenmp -mtune=corei7-avx") 

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Boost_INCLUDE_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/include/)
set(Boost_LIBRARY_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/lib/)

set( ROOT_PROJECT ../ )

include_directories( 
  "${Boost_INCLUDE_DIR}" 
  "${ROOT_PROJECT}/source/include" 
)

link_directories(${Boost_LIBRARY_DIR})

file(GLOB SOURCES ${ROOT_PROJECT}/source/src/*.cpp)

add_executable( clust_dbscan ${SOURCES} main.cpp )
target_link_libraries( clust_dbscan boost_program_options boost_filesystem boost_system boost_regex boost_unit_test_framework  )
