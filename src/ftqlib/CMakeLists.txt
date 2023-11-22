project(FTQuantLibrary)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Wstrict-aliasing -pedantic -Ofast")

add_library(ftqlib library.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)