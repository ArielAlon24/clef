cmake_minimum_required(VERSION 3.10)

project(clef)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/raylib)
include_directories(external/raylib/src)

include_directories(external/miniaudio)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.c)

add_executable(clef ${SOURCES} src/main.c)

target_link_libraries(clef PUBLIC raylib)

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
