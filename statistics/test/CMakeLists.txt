set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)


include_directories( 
  "${Boost_INCLUDE_DIR}" 
  "${ROOT_PROJECT}/source/include" 
  "${ROOT_PROJECT}/external/include"
  "${ROOT_PROJECT}/main/"
)

link_directories(${Boost_LIBRARY_DIR})

file(GLOB SOURCES ${ROOT_PROJECT}/source/src/*.cpp)
file(GLOB EXTERNAL_SOURCES ${ROOT_PROJECT}/external/src/*.cpp)



add_executable( test_statistics ${SOURCES} ${EXTERNAL_SOURCES}  test_runner.cpp test_fisher.cpp test_g2.cpp  test_chisq.cpp test_permutation.cpp test_perform.cpp )

target_link_libraries( test_statistics boost_program_options boost_filesystem boost_system boost_regex boost_unit_test_framework  )

