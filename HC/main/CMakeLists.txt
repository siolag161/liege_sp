set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Boost_INCLUDE_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/include/)
set(Boost_LIBRARY_DIR /home/pdt/dev/thirdparty/boost_1_55_0/target/lib/)

set( ROOT_PROJECT ../ )

include_directories( 
  "${Boost_INCLUDE_DIR}" 
  "${ROOT_PROJECT}/source/include" 
  "${ROOT_PROJECT}/external/include"
  "${PROJECT_SOURCE_DIR}"
)

link_directories(${Boost_LIBRARY_DIR})

file(GLOB SOURCES ${ROOT_PROJECT}/source/src/*.cpp  )
file(GLOB EXTERNAL ${ROOT_PROJECT}/external/src/*.cpp  )
file(GLOB MAIN ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp  )


add_executable( clust_hc ${SOURCES} ${EXTERNAL} ${MAIN} )
target_link_libraries( clust_hc boost_program_options boost_filesystem boost_system boost_regex boost_unit_test_framework  )

