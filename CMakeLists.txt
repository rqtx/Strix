cmake_minimum_required(VERSION 3.10.3)
project(Strix)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set( STRIX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set(STRIX_VERSION_MAJOR 1)
set(STRIX_VERSION_MINOR 0)
set(STRIX_VERSION_PATCH 0)

set(CMAKE_BINARY_DIR ${STRIX_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g -lpthread -D _DEFAULT_SOURCE")
include_directories(src/include)

set(STRIX_SOURCES 
  src/main.c
  src/cli.c
  src/error.c 
  src/packetforge.c 
  src/executor.c 
  src/memutils.c
  src/injector.c
  src/manager.c
  src/planner.c
)


add_executable(Strix ${STRIX_SOURCES})
