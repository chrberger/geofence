# geofence

![C/C++ CI](https://github.com/chrberger/geofence/workflows/C/C++%20CI/badge.svg) [![Build Status](https://travis-ci.org/chrberger/geofence.svg?branch=master)](https://travis-ci.org/chrberger/geofence) [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

geofence is a small and efficient library written in modern C++ library to test whether a coordinate is inside a polygon to realize, for instance, WGS84 geofencing.

geofence is available as single-file, header-only library - just drop geofence.hpp into your project, #include "geofence.hpp", and compile your project with a modern C++ compiler (C++11 or newer).

This library is partially based on the [work](https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html) from W. Randolph Franklin released under MIT license.


## Table of Contents
* [Features](#features)
* [Dependencies](#dependencies)
* [Contributing](#contributing)
* [License](#license)


## Features
* Written in highly portable and high quality C++11
* **Available as header-only, single-file distribution - just drop [geofence.hpp](https://raw.githubusercontent.com/chrberger/geofence/master/geofence.hpp) into your project, `#include "geofence.hpp"`, and compile your project with a modern C++ compiler (C++11 or newer)**
* The polygon and position are passed to the functions as [`std::array`](http://en.cppreference.com/w/cpp/container/array) so that this library integrates well with other math libraries (e.g., Eigen).


## Dependencies
No dependencies! All you need is a C++11-compliant compiler as the project ships the following dependencies as part of the source distribution:

* [Unit Test Framework Catch2](https://github.com/catchorg/Catch2/releases/tag/v2.12.1) - [![License: Boost Software License v1.0](https://img.shields.io/badge/License-Boost%20v1-blue.svg)](http://www.boost.org/LICENSE_1_0.txt) - [Source](https://github.com/chrberger/stringtoolbox/blob/master/test/catch.hpp)


## Installation
### Installation as single-file, header-only library
WGS84toCartesian is provided as header-only, single-file library as well - just drop [WGS84toCartesian.hpp](https://raw.githubusercontent.com/chrberger/WGS84toCartesian/master/WGS84toCartesian.hpp) into your project, `#include "WGS84toCartesian.hpp"`, and compile your project with a modern C++ compiler (C++11 or newer)


## Projects using geofence
If your project is using geofence, just let us know :-)


## Contributing
We are happy to receive your PRs to accelerate libcluon's development; before contributing, please take a look at the [Contribution Documents](CONTRIBUTING.md).


## License
* This project is released under the terms of the MIT License - [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
