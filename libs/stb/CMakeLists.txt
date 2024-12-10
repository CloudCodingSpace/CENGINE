cmake_minimum_required(VERSION 3.5.0)
cmake_policy(SET CMP0069 NEW)
project(stb)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(stb)
target_sources(stb PRIVATE ${SRCS})
target_include_directories(stb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")