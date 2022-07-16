cmake_minimum_required(VERSION 3.0.0)
project(pkclient VERSION 0.1.0)

include_directories(
    C:/vcpkg/installed/x64-windows/include
)

link_directories(
   C:/vcpkg/installed/x64-windows/lib
)

add_subdirectory(../pkCommon ../pkCommon/build)

add_executable(pkclient 
    main.cpp
    game.cpp
    chatbuffer.cpp
    netclient.cpp
    phasestatemachine.cpp
    menuphase.cpp
    gamephase.cpp
)

target_link_libraries(pkclient
    raylib
    pkcommon
)
