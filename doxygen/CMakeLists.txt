find_package(Doxygen REQUIRED)
if (DOXYGEN_FOUND)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_LOCAL_CLASSES YES)
    set(DOXYGEN_FILE_PATTERNS *.hpp *.cpp)
    set(DOXYGEN_HIDE_FRIEND_COMPOUNDS YES)
    set(DOXYGEN_HIDE_IN_BODY_DOCS NO)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_REFERENCED_BY_RELATION YES)
    set(DOXYGEN_REFERENCES_RELATION YES)
    set(DOXYGEN_SORT_BY_SCOPE_NAME YES)
    set(DOXYGEN_SORT_MEMBER_DOCS YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/documentation")

    message(STATUS "Doxygen:                  ${DOXYGEN_EXECUTABLE}")
    message(STATUS "Doxygen version:          ${DOXYGEN_VERSION}")
    message(STATUS "Doxygen output directory: ${DOXYGEN_OUTPUT_DIRECTORY}")

    doxygen_add_docs(Doxygen
            "${CMAKE_SOURCE_DIR}/src"
            "${CMAKE_SOURCE_DIR}/tests"
            "${CMAKE_SOURCE_DIR}/ftqlib"
            "${CMAKE_SOURCE_DIR}/include"
            ALL
            COMMENT "Generate HTML documentation")
else (DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif (DOXYGEN_FOUND)