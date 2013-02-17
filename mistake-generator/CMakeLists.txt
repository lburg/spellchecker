cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c99 -W -Wall -Wextra")
add_definitions(-D_GNU_SOURCE)

set(SOURCES_DIR sources)

list(APPEND MISTAKE_GENERATOR_SOURCES
    ${SOURCES_DIR}/main.c
    )

add_executable(mistake-generator ${MISTAKE_GENERATOR_SOURCES})
