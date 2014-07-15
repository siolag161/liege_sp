set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)


set(Boost_INCLUDE_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/include/)
set(Boost_LIBRARY_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/lib/)

set(ProBT_INCLUDE_DIR /home/pdt/dev/thirdparty/probt_spl_2_3_0/include/)
set(ProBT_LIBRARY_DIR /home/pdt/dev/thirdparty/probt_spl_2_3_0/lib/ )

include_directories( 
  "${ProBT_INCLUDE_DIR}"
  "${Boost_INCLUDE_DIR}"
  "${PROJECT_SOURCE_DIR}/source/include" 
  "${PROJECT_SOURCE_DIR}/external/include"
)

link_directories(${Boost_LIBRARY_DIR} ${ProBT_LIBRARY_DIR} )

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/source/src/*.cpp)
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable( test_clust_cmp ${SOURCES} ${TEST_SOURCES} )
target_link_libraries( test_clust_cmp  boost_program_options boost_filesystem boost_system boost_regex boost_unit_test_framework spl)

