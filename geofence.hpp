/*
 * MIT License
 *
 * Copyright (c) 2020  Christian Berger
 * Copyrihgt (c) 1970  W. Randolph Franklin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// The algorithms is based on W. Randolph Franklin's implementation that can be found here:
// https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html

#ifndef GEOFENCE_HPP
#define GEOFENCE_HPP

#include <cmath>
#include <cstdint>
#include <array>
#include <type_traits>
#include <vector>

namespace geofence {

/**
 * @param polygon describing a geofenced area
 * @param p point to test whether inside or not
 * @return true if p is inside polygon
 */
template <typename T>
inline bool isIn(std::vector<std::array<T,2>> &polygon, std::array<T,2> &p) {
  static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
  bool inside{false};
  if (0 < polygon.size()) {
    constexpr const uint8_t X{0};
    constexpr const uint8_t Y{1};
    const std::size_t POINTS{polygon.size()};
    std::size_t i{0};
    std::size_t j{POINTS - 1};
    for(; i < POINTS ; j = i++) {
      if ( ((polygon.at(i)[Y] > p[Y]) != (polygon.at(j)[Y] > p[Y])) &&
           (p[X] < (polygon.at(j)[X]-polygon.at(i)[X]) * (p[Y]-polygon.at(i)[Y]) / (polygon.at(j)[Y]-polygon.at(i)[Y]) + polygon.at(i)[X]) ) {
        inside = !inside;
      }
    }
  }
  return inside;
}

}
#endif
