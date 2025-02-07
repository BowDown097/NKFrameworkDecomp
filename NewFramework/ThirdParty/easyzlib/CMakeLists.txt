cmake_minimum_required(VERSION 3.5)
project(easyzlib C)

add_library(easyzlib easyzlib.c)
target_include_directories(easyzlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
