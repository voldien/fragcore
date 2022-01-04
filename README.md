# FragCore - Work in Progress
[![Linux Build](https://github.com/voldien/fragcore/actions/workflows/linux-build.yml/badge.svg)](https://github.com/voldien/fragcore/actions/workflows/linux-build.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/voldien/fragcore.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/voldien/fragcore/context:cpp)
[![GitHub release](https://img.shields.io/github/release/voldien/fragcore.svg)](https://github.com/voldien/fragcore/releases)

A Core library framework library for Game Engine and other similar software.

Architecture is still in work progress.

## Features

* IO - Abstract interface for IO, such as File,Memory,TCP/UDP,FTP and much more
* Modular - Modular support of loading library in runtime and compilation time.
* Network - Support creating network sockets,
* DataStructure - Common data structure in programming as well game development.
* Threading - Cross platform thread support.
* Task Scheduler - Task scheduler for cross platform for uniform CPU resource utilization.
* Math - Vector math, and common math functions.

## Motivation

This library contains all the low level functionlties assocated with a game engine. It was derived from the project fragview and fragengine, extracted their common low level functionties.

## Installation

First, download the repository, with the followig command.
it will download the repository along with all the dependent git submodules it uses.

```bash
git clone --recurse-submodules https://github.com/voldien/fragcore.git
```

The software can be easily installed with invoking the following command.

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

```bash
mkdir build && cd build
cmake ..  -DCMAKE_BUILD_TYPE=Debug  -DBUILD_WITH_COVERAGE=ON -DBUILD_WITH_TEST=ON -DBUILD_WITH_UBSAN=ON -DBUILD_WITH_ASAN=ON 
cmake --build .
make install
```

## Dependencies

In order to compile the program, the following Debian packages is required.

```bash
apt install libyaml-dev libjson-c-dev libxml2-dev libsdl2-dev libglew-dev libvulkan-dev libgl1-mesa-dev opencl-headers libzip-dev libfswatch-dev libfreeimage-dev libavcodec-dev libavfilter-dev libavformat-dev  libassimp-dev libfreetype6-dev  libspdlog-dev
  sudo apt-get install -y libegl1-mesa-dev libgles2-mesa-dev
  sudo apt-get install -y libglew-dev libgtest-dev libsdl2-dev libsubunit-dev
  sudo apt-get install -y libsdl2-dev libglew-dev libvulkan-dev libgl1-mesa-dev opencl-headers libzip-dev libfswatch-dev libfreeimage-dev libfswatch-dev libxml2-dev 
```

```bash
apt install libopenal-dev libalut-dev libbullet-dev
```

## Assert

Tool dependencies.

```bash
apt install zip gzip libgtest-dev 
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
