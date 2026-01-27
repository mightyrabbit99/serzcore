cmake_minimum_required(VERSION 3.23)

# Define the project
project(serzcore)
set(serzcore_VERSION_MAJOR 1)  #version number
set(serzcore_VERSION_MINOR 0)

option(BUILD_EXAMPLES "build examples" OFF)
set(LUA_LIBRARY "" CACHE STRING "lua library")
set(LUA_INCLUDE_DIR "" CACHE STRING "lua header directory")
if(NOT "${LUA_LIBRARY}" STREQUAL "")
if("${LUA_INCLUDE_DIR}" EQUAL "")
message(FATAL_ERROR "LUA_INCLUDE_DIR needed")
endif()
set(BUILD_SERZCORE_LUA ON)
endif()

add_subdirectory(src)
if (BUILD_EXAMPLES)
  add_subdirectory(examples/traffic)
  add_subdirectory(examples/hello)
endif ()
