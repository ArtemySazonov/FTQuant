set(SOURCES main.cpp SyntaxParser.cpp)

add_executable(FTQS ${SOURCES})
target_link_libraries(FTQS PRIVATE
        ftqlib
)