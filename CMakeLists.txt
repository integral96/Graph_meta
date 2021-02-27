cmake_minimum_required(VERSION 3.5)

project(Project_math  ${PROJECT_NAME} VERSION "0.0.0" )


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -bigobj")


file(GLOB SOURCES
    include/base_func.hpp
    include/lagrange_group.hpp
    include/graph.hpp
    include/matrix.hpp
    include/simvolic_diff.hpp
    include/testBB.hpp
    *.h.in

    main.cpp)
file(GLOB GRAPH_LIB
    include/base_func.hpp
    include/lagrange_group.hpp
    include/graph.hpp
    include/matrix.hpp
    include/simvolic_diff.hpp
    graph.cpp)

#set(TBB_DEFAULT_SEARCH_DIR "/opt/intel/tbb")

#find_package(TBB REQUIRED tbb tbbmalloc tbbmalloc_proxy)
#if (TBB_FOUND)
#    message(STATUS "Found Intel Thread Building Blocks TBB library "
#      ${TBB_VERSION} " assuming SuiteSparseQR was compiled "
#      "(version: ${TBB_VERSION_MAJOR}.${TBB_VERSION_MINOR}, interface: "
#      "${TBB_INTERFACE_VERSION}) assuming SuiteSparseQR was compiled "
#      "with TBB.")
#    # Add the TBB libraries to the SuiteSparseQR libraries (the only
#    # libraries to optionally depend on TBB).
#    list(APPEND SUITESPARSEQR_LIBRARY ${TBB_LIBRARIES})
#    message(STATUS "TBB_LIBRARIES: ${TBB_LIBRARIES}")
#  else()
#    message(STATUS "Did not find Intel TBB library, assuming SuiteSparseQR was "
#      "not compiled with TBB.")
#  endif()

find_package(Boost 1.75.0 COMPONENTS date_time timer coroutine thread system unit_test_framework regex REQUIRED)

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/include/version.h)

add_executable(${PROJECT_NAME} ${SOURCES})

add_library(lib${PROJECT_NAME} lib.cpp)
#add_library(Graph_lib${PROJECT_NAME} ${GRAPH_LIB})

add_executable(test_version test_version.cpp)

set(GRAPH graph_test)
add_executable(${GRAPH} ${GRAPH_LIB} )

set_target_properties(${PROJECT_NAME} lib${PROJECT_NAME} test_version PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wall;-Wextra;"
)

set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
    lib${PROJECT_NAME}
)

target_link_libraries(test_version
    ${Boost_LIBRARIES}
    lib${PROJECT_NAME}
)

target_link_libraries(${GRAPH}
    ${Boost_LIBRARIES}
)

enable_testing ()

add_test(helloworld_tests test_version)
