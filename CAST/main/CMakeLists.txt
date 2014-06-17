
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Boost_INCLUDE_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/include/)
set(Boost_LIBRARY_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/lib/)

set( ROOT_PROJECT ../source )

include_directories( 
  "${Boost_INCLUDE_DIR}" 
  "${ROOT_PROJECT}/include" 
  "./"
)

MESSAGE( ${PROJECT_SOURCE_DIR}/source/src )  
link_directories(${Boost_LIBRARY_DIR})

file(GLOB SOURCES ${ROOT_PROJECT}/src/*.cpp)

add_executable( clust_cast ${SOURCES} main.cpp )
target_link_libraries( clust_cast boost_program_options boost_filesystem boost_system boost_regex )