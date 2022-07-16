cmake_minimum_required(VERSION 3.0.0)
project(pkcommon VERSION 0.1.0)

add_library(pkcommon 
	netcmd.cpp
)

target_include_directories (pkcommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
