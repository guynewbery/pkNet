cmake_minimum_required(VERSION 3.0.0)
project(common VERSION 0.1.0)

add_library(common 
	pknet.cpp
)

target_include_directories (common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
