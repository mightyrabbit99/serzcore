cmake_minimum_required(VERSION 3.13)

# Define the project
project(serzcore)
set(serzcore_VERSION_MAJOR 1)  #version number
set(serzcore_VERSION_MINOR 0)

option(BUILD_EXAMPLES "build examples" OFF)

add_subdirectory(src)
if (BUILD_EXAMPLES)
  add_subdirectory(examples/traffic)
endif ()
