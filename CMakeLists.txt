cmake_minimum_required(VERSION 3.20)
project(FTQuant LANGUAGES CXX VERSION 0.1)

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3")

message(STATUS "\nSystem Info")
message(STATUS "CMAKE_SYSTEM_NAME:        ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR:   ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_CXX_COMPILER:       ${CMAKE_CXX_COMPILER}")

message(STATUS "\nBuild Options")
message(STATUS "CMAKE_CXX_STANDARD:       C++${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_CXX_FLAGS:         ${CMAKE_CXX_FLAGS}")
message(STATUS "BUILD_DOC:                ${BUILD_DOC}")
message(STATUS "BUILD_TESTS:              ${BUILD_TESTS}")
message(STATUS "Boost_FOUND:              ${Boost_FOUND}")

add_subdirectory(ftqlib)
add_subdirectory(src)
if(BUILD_DOC)
    add_subdirectory(doxygen)
endif()
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

