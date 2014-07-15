set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include_directories( 
  "${Boost_INCLUDE_DIR}" 
  "${ROOT_PROJECT}/source/include" 
  "${ROOT_PROJECT}/external/include"
  "./"
)

link_directories(${Boost_LIBRARY_DIR})

file(GLOB SOURCES ${ROOT_PROJECT}/source/src/*.cpp)
file(GLOB EXTERNAL_SOURCES ${ROOT_PROJECT}/external/src/*.cpp)
add_executable( statistics ${SOURCES} ${EXTERNAL_SOURCES} statistics.cpp )

target_link_libraries( statistics boost_program_options boost_filesystem boost_system boost_regex )

