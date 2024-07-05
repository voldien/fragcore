# FragCore

[![Linux Build](https://github.com/voldien/fragcore/actions/workflows/linux-build.yml/badge.svg)](https://github.com/voldien/fragcore/actions/workflows/linux-build.yml)
[![License: LGPL v3](https://img.shields.io/badge/License-LGPLv3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![GitHub release](https://img.shields.io/github/release/voldien/fragcore.svg)](https://github.com/voldien/fragcore/releases)

A Core library framework library developed for both, but not exclusivly for Game Engines.

## Architecture
The architecture of this library consist of a core library and modules. The core contains all the basic low level system logic. This core component are used for all the modules. These modules are dedicated library that extend the feature set of the fragcore.

## Features

* **Core** - Contains all the basic functionalities and logic for modular design.
* **Modular** - Modular support of loading library in runtime and compilation time.
* **IO** - Abstract interface for IO, such as File,Memory,TCP/UDP,FTP and much more
* **Network** - Support creating network sockets,
* **DataStructure** - Common data structure in programming as well game development.
* **Threading** - Cross platform thread support.
* **Task Scheduler** - Task scheduler for cross platform for uniform CPU resource utilization.
* **Math** - Vector (3D) math, and common math functions used in computer games.
* **ASync** - Support IO Async read/write

## Motivation

The intention is to create a library that contains all the low-level functionalities associated with a game engine. That way it can be integrated with many other projects, all from basic to very advanced software.
It was derived from the projects fragview, physic-engine and fragengine, extracted their common low-level functionality.

## Installation

First, download the repository, with the following command. It will download the repository along with all the dependent git submodules it uses.

```bash
git clone --recurse-submodules https://github.com/voldien/fragcore.git
```

The software can be easily installed by invoking the following command.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Integration with CMake

The idea is to be able to integrate this library with another project easily. With CMake, it basically requires 2 lines. One for adding the project and the second for adding it as a dependent linked library target.

```cmake
ADD_SUBDIRECTORY(fragcore EXCLUDE_FROM_ALL)
```

Assgin the library target to the your target, and all the depedencies of fragcore will be configured and built.
```cmake
TARGET_LINK_LIBRARIES(myTarget PUBLIC fragcore)
```

## Development Build

For development, using additional optional flags can be used for securing software is working and reducing bugs etc.

```bash
mkdir build && cd build
cmake ..  -DCMAKE_BUILD_TYPE=Debug  -DBUILD_WITH_COVERAGE=ON -DBUILD_WITH_TEST=ON -DBUILD_WITH_UBSAN=ON -DBUILD_WITH_ASAN=ON 
cmake --build .
make install
```

## Dependencies

In order to compile the program on Linux based machine, the following Debian packages are required.

```bash
  sudo apt-get install g++-10 pkg-config libjson-c-dev libxml2-dev libyaml-dev libsdl2-dev libfreeimage-dev libssl-dev
  sudo apt-get install libsdl2-dev libglew-dev libvulkan-dev libgl1-mesa-dev opencl-headers \
  libgtest-dev libsubunit-dev libogg-dev libopus-dev libvorbis-dev \
  libzip-dev libbullet-dev libopenal-dev mesa-common-dev ocl-icd-opencl-dev libnoise-dev libvdpau-dev \
  libalut-dev libspdlog-dev libbz2-dev libcurl4 libcurl4-gnutls-dev libfmt-dev binutils-dev libeigen3-dev
```

## Testing

Tool dependencies.

```bash
apt install zip gzip libgtest-dev 
```

```bash
mkdir build && cd build
cmake ..
make 
ctest
```

## Modules

The project is design that modules can be use to reduce both dependices and code depdending on what the library is integrate with. Currently, this can be done with CMake. The modules can be located in the [modules](modules) directory.

- **Audio** - Currently it supports OpenAL.
- **Geometry Util** - 
- **Image Loader** - Load/Save Image.
- **IO** - File, Memory, Modbus, Serial, TCP/UDP
- **Physic** - Physic Simulator for Game Engines. Bullet/PhyX/RPC
- **Renderer** - Vulkan/OpenGL
- **Window** - SDL/GLFW
- **TaskScheduler** - Task scheduler for distrubte tasks on CPU.

## License

This project is licensed under the LGPL+3 License - see the [LICENSE](LICENSE) file for details
