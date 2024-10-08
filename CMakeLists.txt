cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

project(src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/neon)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/neon/src)

list(APPEND LIBRARIES neon)

include_directories(src)
add_subdirectory(src)