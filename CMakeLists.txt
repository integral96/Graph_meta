cmake_minimum_required(VERSION 3.5)

project(Graph  ${PROJECT_NAME} VERSION "0.0.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -bigobj")

file(GLOB SOURCES
    include/base_func.hpp
    include/lagrange_group.hpp
    include/graph.hpp
    include/matrix.hpp
    *.h.in

    main.cpp)

find_package(Boost 1.75.0 COMPONENTS date_time timer thread system unit_test_framework regex REQUIRED)

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/include/version.h)

add_executable(${PROJECT_NAME} ${SOURCES})

add_library(lib${PROJECT_NAME} lib.cpp)

add_executable(test_version test_version.cpp)

set_target_properties(${PROJECT_NAME} lib${PROJECT_NAME} test_version PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
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

enable_testing ()

add_test(helloworld_tests test_version)
