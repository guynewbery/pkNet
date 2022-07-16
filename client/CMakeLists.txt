cmake_minimum_required(VERSION 3.0.0)
project(client VERSION 0.1.0)

include_directories(
    C:/vcpkg/installed/x64-windows/include
)

link_directories(
   C:/vcpkg/installed/x64-windows/lib
)

add_subdirectory(../common ../common/build)

add_executable(client 
    main.cpp
    game.cpp
    chatbuffer.cpp
    netclient.cpp
    phasestatemachine.cpp
    menuphase.cpp
    gamephase.cpp
)

target_link_libraries(client
    raylib
    common
)
