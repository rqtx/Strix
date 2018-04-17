cmake_minimum_required(VERSION 3.10.3)
project(Strix)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c14 -Wall")

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(Strix ${PROJECT_SOURCE_DIR}/src/main.c)
