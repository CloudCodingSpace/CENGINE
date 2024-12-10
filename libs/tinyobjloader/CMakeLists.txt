cmake_minimum_required(VERSION 3.5.0)
cmake_policy(SET CMP0069 NEW)
project(tinyobj)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")