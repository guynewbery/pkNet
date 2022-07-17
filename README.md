# pkNet

A toy game engine

# Setup

  - Install chocolatey (https://chocolatey.org/install)

  - Install git
```c
choco install git.install
```

  - Install Visual Studio Code (https://code.visualstudio.com/download)

  - Install Microsoft C++ Build Tools, making sure to install MSVC build tools and Windows 10 SDK (https://visualstudio.microsoft.com/visual-cpp-build-tools/)

  - Install vcpkg
```c
cd c:\
git clone https://github.com/microsoft/vcpkg
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
```

  - Install raylib
```c
c:\vcpkg\vcpkg install raylib:x64-windows
```

  - Install rttr
```c
c:\vcpkg\vcpkg install rttr:x64-windows
```

  - Configure vcpkg for Visual Studio Code (https://gamefromscratch.com/vcpkg-cpp-easy-mode-step-by-step-tutorial/)


# Resources

https://code.visualstudio.com/docs/cpp/config-msvc

https://gamefromscratch.com/vcpkg-cpp-easy-mode-step-by-step-tutorial/

https://gafferongames.com/categories/game-networking/

https://gafferongames.com/categories/building-a-game-network-protocol/
