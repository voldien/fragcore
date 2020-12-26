# FragCore
[![Actions Build Status](https://github.com/voldien/fragcore/workflows/fragcore/badge.svg?branch=master)](https://github.com/voldien/fragcore/actions)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![GitHub release](https://img.shields.io/github/release/voldien/fragcore.svg)](https://github.com/voldien/fragcore/releases)
<!-- TODO, LGMT, version  -->

[image]

# Work in Progress

## Installation
The software can be easily installed with invoking the following command.
```bash
mkdir build && cd build
cmake ..
cmake --build .
make install
```

## Examples

```c

```

# Dependencies #
In order to compile the program, the following Debian packages is required. 

The following command will display current version of gcc.
```bash
gcc --version
```
## Dependencies ##
```bash
apt install libyaml-dev libjson-c-dev libxml2-dev libsdl2-dev libglew-dev libvulkan-dev libgl1-mesa-dev opencl-headers libzip-dev libfswatch-dev libfreeimage-dev libavcodec-dev libavfilter-dev libavformat-dev  libassimp-dev libfreetype6-dev 
  sudo apt-get install -y libegl1-mesa-dev libgles2-mesa-dev
  sudo apt-get install -y libglew-dev libgtest-dev libsdl2-dev libsubunit-dev
  sudo apt-get install -y libsdl2-dev libglew-dev libvulkan-dev libgl1-mesa-dev opencl-headers libzip-dev libfswatch-dev libfreeimage-dev libfswatch-dev libxml2-dev
```

```bash
apt install libopenal-dev libalut-dev libbullet-dev
```

### Assert ###
Tool dependencies.
```bash
apt install zip gzip libgtest-dev 
```

## Software using the library

* Fvecfield: []()
* konafrag: []()
* mandelbrot: []()
* fragview: []()


## License
This project is licensed under the GPL+3 License - see the [LICENSE](LICENSE) file for details
