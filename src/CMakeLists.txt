set(SOURCES main.cpp)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(FTQS ${SOURCES})
target_link_libraries(FTQS PRIVATE
        ftqlib
)