cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -W -Wall -Wextra")
add_definitions(-D_GNU_SOURCE)

set(SOURCES_DIR sources)
set(INCLUDE_DIR include)

include_directories(
    ${INCLUDE_DIR}
    )

list(APPEND SPELLCHECKER_SOURCES
    ${SOURCES_DIR}/trie.c
    ${SOURCES_DIR}/spellcheck.c
    ${SOURCES_DIR}/autocomplete.c
    ${SOURCES_DIR}/main.c
    )

add_executable(spellchecker ${SPELLCHECKER_SOURCES})
