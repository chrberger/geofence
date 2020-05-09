/*
 * MIT License
 *
 * Copyright (c) 2020  Christian Berger
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

#ifndef GEOFENCE_HPP
#define GEOFENCE_HPP

#include <cmath>
#include <cstdint>
#include <algorithm>
#include <array>
#include <limits>
#include <type_traits>
#include <vector>

namespace geofence {

/*
Further aspects to consider:
- test if point is outside bounding box --> also outside polygon (DONE)
- repair self-intersecting polygon to non-self-intersecting polygon (DONE)
- test if point is vertex --> true (DONE)
- test if point is on edge --> true
- run WRF algorithm
*/

/**
 * @param a
 * @param b
 * @param c
 * @return true if c is on the line defined by a and b and also between a and b
 */
template <typename T>
inline bool isInBetween(std::array<T,2> a, std::array<T,2> b, std::array<T,2> c) {
  constexpr const auto EPSILON = 1.0e-09f;
  constexpr const uint8_t X{0};
  constexpr const uint8_t Y{1};

  const T crossproduct = (c[Y] - a[Y]) * (b[X] - a[X]) - (c[X] - a[X]) * (b[Y] - a[Y]);
  if (std::abs(crossproduct) > EPSILON) {
    return false;
  }

  const T dotproduct = (c[X] - a[X]) * (b[X] - a[X]) + (c[Y] - a[Y]) * (b[Y] - a[Y]);
  if (dotproduct < 0) {
    return false;
  }

  const T squaredLengthBA = (b[X] - a[X]) * (b[X] - a[X]) + (b[Y] - a[Y]) * (b[Y] - a[Y]);
  if (dotproduct > squaredLengthBA) {
    return false;
  }

  return true;
}

/**
 * @param axis-aligned boundingbox
 * @param p point
 * @return true if point is inside this rectangle to enable quick testing
 */
template <typename T>
inline bool isInAxisAlignedBoundingBox(std::vector<std::array<T,2>> &bb, std::array<T,2> p) {
  static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
  bool inside{false};
  if (2 == bb.size()) {
    constexpr const uint8_t X{0};
    constexpr const uint8_t Y{1};
    inside = ((bb.at(0)[X] < p[X]) && (p[X] < bb.at(1)[X])) &&
             ((bb.at(0)[Y] > p[Y]) && (p[Y] > bb.at(1)[Y]));
  }
  return inside;
}

/**
 * @param polygon with more than one vertex
 * @return axis-aligned bounding box, which can be used for quick testing whether point is inside this rectangle
 */
template <typename T>
inline std::vector<std::array<T,2>> getAxisAlignedBoundingBox(std::vector<std::array<T,2>> &polygon) {
  static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
  std::vector<std::array<T,2>> bb;
  if (1 < polygon.size()) {
    std::array<T,2> TL{(std::numeric_limits<T>::max)(), (std::numeric_limits<T>::min)()};
    std::array<T,2> BR{(std::numeric_limits<T>::min)(), (std::numeric_limits<T>::max)()};
    constexpr const uint8_t X{0};
    constexpr const uint8_t Y{1};
    for(auto p : polygon) {
      if (p[X] < TL[X]) {
        TL[X] = p[X];
      }
      if (p[Y] > TL[Y]) {
        TL[Y] = p[Y];
      }
      if (BR[X] < p[X]) {
        BR[X] = p[X];
      }
      if (BR[Y] > p[Y]) {
        BR[Y] = p[Y];
      }
    }
    bb.push_back(TL);
    bb.push_back(BR);
  }
  return bb;
}

/**
 * @param a
 * @param b
 * @return true if a and b are (almost - in case of floating points) equal
 */
template <typename T>
inline bool isEqual(T a, T b) {
  // Inspired by: https://www.embeddeduse.com/2019/08/26/qt-compare-two-floats/
  static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wabsolute-value"
#endif
  constexpr auto EPSILON = 1.0e-09f;
  return (std::abs(a - b) <= EPSILON) ? true : std::abs(a - b) <= EPSILON * (std::max)(std::abs(a), std::abs(b));
#pragma GCC diagnostic pop
}

/**
 * Compute convex hull using Andrew's monotone chain algorithm.
 * @param polygon
 * @return convex hull
 */
template <typename T>
inline bool getConvexHull(const std::vector<std::array<T,2>> &polygon) {
  static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

  // Inspired by: https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain#C++ 

  auto isLeft = [](const std::array<T,2> &a, const std::array<T,2> &b) {
    constexpr const uint8_t X{0};
    constexpr const uint8_t Y{1};
	  return (a[X] < b[X] || (a[X] == b[X] && a[Y] < b[Y]));
  };

  auto ccw = [](const std::array<T,2> &a, const std::array<T,2> &b, const std::array<T,2> &c) {
    constexpr const uint8_t X{0};
    constexpr const uint8_t Y{1};
	  return (b[X] - a[X]) * (c[Y] - a[Y]) - (b[Y] - a[Y]) * (c[X] - a[X]);
  };

  auto sortedPolygon{polygon};
  std::sort(sortedPolygon.begin(), sortedPolygon.end(), isLeft);

  // Construct lower half of convex hull.
  std::vector<std::array<T,2>> lowerHalf;
  for(auto it{sortedPolygon.begin()}; it != sortedPolygon.end(); ++it) {
    while(lowerHalf.size() >= 2 && (!ccw(*(lowerHalf.rbegin()+1), *(lowerHalf.rbegin()), *it) < 0)) {
      lowerHalf.pop_back();
    }
    lowerHalf.push_back(*it);
  }

  // Construct upper half of convex hull.
  std::vector<std::array<T,2>> upperHalf;
  for(auto it{sortedPolygon.rbegin()}; it != sortedPolygon.rend(); ++it) {
    while(upperHalf.size() >= 2 && (!ccw(*(upperHalf.rbegin()+1), *(upperHalf.rbegin()), *it) < 0)) {
      upperHalf.pop_back();
    }
    upperHalf.push_back(*it);
  }

  std::vector<std::array<T,2>> convexHull;
	convexHull.insert(convexHull.end(), lowerHalf.begin(), lowerHalf.end());
	convexHull.insert(convexHull.end(), upperHalf.begin() + 1, upperHalf.end() - 1);
  return convexHull;
}

/**
 * @param polygon describing a geofenced area
 * @param p point to test whether inside or not
 * @return true if p is inside the convex hull for the polygon OR when p is any vertex OR on an edge of the convex hull
 */
template <typename T>
inline bool isIn(std::vector<std::array<T,2>> &polygon, std::array<T,2> &p) {
  static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
  bool inside{false};
  if (2 < polygon.size()) {
    constexpr const uint8_t X{0};
    constexpr const uint8_t Y{1};
    const std::size_t POINTS{polygon.size()};
    std::size_t i{0};
    std::size_t j{POINTS - 1};
    for(; i < POINTS ; j = i++) {
      if ( isEqual(p[X], polygon.at(i)[X]) && isEqual(p[Y], polygon.at(i)[Y]) ) {
        return true;
      }

      // The algorithms is based on W. Randolph Franklin's implementation that can be found here:
      // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
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
