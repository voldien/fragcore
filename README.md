# FragCore - Work in Progress
[![Linux Build](https://github.com/voldien/fragcore/actions/workflows/linux-build.yml/badge.svg)](https://github.com/voldien/fragcore/actions/workflows/linux-build.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/voldien/fragcore.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/voldien/fragcore/context:cpp)
[![GitHub release](https://img.shields.io/github/release/voldien/fragcore.svg)](https://github.com/voldien/fragcore/releases)

A Core library framework library for Game Engine and other similar software that can benefit from modularity.

Architecture is still in work progress.

## Features

* **IO** - Abstract interface for IO, such as File,Memory,TCP/UDP,FTP and much more
* **Modular** - Modular support of loading library in runtime and compilation time.
* **Network** - Support creating network sockets,
* **DataStructure** - Common data structure in programming as well game development.
* **Threading** - Cross platform thread support.
* **Task** Scheduler - Task scheduler for cross platform for uniform CPU resource utilization.
* **Math** - Vector (3D) math, and common math functions used in computer games.
* **ASync** - Support IO Async read/write

## Motivation

This library contains all the low-level functionalities associated with a game engine. It was derived from the projects fragview, physic-engine and fragengine, extracted their common low-level functionality.

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
make install
```

## Integration with CMake

The idea is to be able to integrate this library with another project easily. With CMake, it basically requires 2 lines. One for adding the project and the second for adding it as a dependent linked library target.

```cmake
ADD_SUBDIRECTORY(fragcore EXCLUDE_FROM_ALL)
```

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

The project is design that modules can be use to reduce both dependices and code depdending on what the library is integrate with. Currently, this can be done with CMake.


### Audio

Currently it supports OpenAL and FMOD.s

#### AudioDecoder

#### OpenAL

```bash
apt install libopenal-dev libalut-dev
```

#### FMOD

### Rendering Interface

A rendering interface is being developed that allows multiple rendering interfaces to be under common interface object.

It currently support Vulkan, OpenGL and partially OpenCL.

#### OpenGL

```bash
apt install 
```

#### Vulkan

```bash
apt install 
```

### VideoPlayback

Video play is supported modules, which requires both currently require both the audio and rendering interface.

```bash
apt install 
```

### Physic Interface

#### Bullet

```bash
apt install libbullet-dev
```

#### PhysX

Nvidia PhysX

#### RPC

Remote procedure call, remote physic,

### Font

```bash
apt install libfreetype-dev 
```

### Communication Protocols

There are couple communication protocol supported. Modbus,CAN, Serial, CURL (FTP)

#### Modbus

```bash
apt install libmodbus-dev
```

#### Serial

```bash
apt install libserialport-dev 
```

#### Curl FileSystem

```bash
apt install 
```




## Software using the library

The following are example library/executables that are using the library.

* **Fvecfield:** []()
* **mandelbrot:** []()
* **fragview:** []()
* **fragengine** []()
* **vulkan-samples** []()

## License

This project is licensed under the GPL+3 License - see the [LICENSE](LICENSE) file for details
