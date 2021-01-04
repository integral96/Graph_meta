cmake_minimum_required(VERSION 3.5)

project(Graph  ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -bigobj")

file(GLOB SOURCES
    include/*.hpp
    *.cpp)
find_package(Boost 1.75.0 COMPONENTS context coroutine date_time timer thread system program_options regex)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES} )
