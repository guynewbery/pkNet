cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0)

INCLUDE_DIRECTORIES(
    C:/vcpkg/installed/x64-windows/include
)

LINK_DIRECTORIES(
   C:/vcpkg/installed/x64-windows/lib
) 

add_subdirectory(../common ../common/build)

add_executable(server 
    main.cpp
)

TARGET_LINK_LIBRARIES(server
    raylib
	common
)
