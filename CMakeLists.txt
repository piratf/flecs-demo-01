cmake_minimum_required(VERSION 3.25)
project(flecs_demo_01)

set(CMAKE_CXX_STANDARD 14)

include(cmake/GPerfTools.cmake)
include(cmake/ThirdParty.cmake)

add_subdirectory(src)