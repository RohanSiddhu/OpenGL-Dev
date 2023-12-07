# lights

## Build
### Tools
1. CMake (minimum version 3.18)
2. Compatible C/C++ 64-bit compiler (Recomanded - GCC, Clang or MSVC)
### Fedora
1. Install Dependencies

```
sudo dnf install extra-cmake-modules \
mesa-libGL-devel \
wayland-devel \
wayland-protocols-devel \
libxkbcommon-devel \
libXrandr-devel \
libXinerama-devel \
libXcursor-devel \
libXi-devel
```

2. Create build directory in project's root and build using cmake

```
mkdir build && cd build
cmake ..
cmake --build .
```

5. Now run ```./lights```
