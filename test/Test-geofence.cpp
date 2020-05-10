/*
 * MIT License
 *
 * Copyright (c) 2018  Christian Berger
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

#include "catch.hpp"

#include <iostream>

#include "geofence.hpp"

TEST_CASE("equality checks") {
  CHECK(geofence::isEqual<uint16_t>(15, 15));
  CHECK(!geofence::isEqual<int16_t>(15, -15));
  CHECK(!geofence::isEqual<float>(0, 0.001f));
  CHECK(geofence::isEqual<float>(0.0009f, 0.0009f));
  CHECK(geofence::isEqual<double>(0.0000009f, 0.0000009f));
}

///////////////////////////////////////////////////////////////////////////////

TEST_CASE("non-poplygon returns false") {
  std::vector<std::array<int,2>> polygon;
  std::array<int,2> point{1, 2};
  CHECK(!geofence::isIn<int>(polygon, point));
}

TEST_CASE("one-point-poplygon returns false") {
  std::vector<std::array<int,2>> polygon;
  std::array<int,2> a{0,0};
  polygon.push_back(a);
  std::array<int,2> point{1, 2};
  CHECK(!geofence::isIn<int>(polygon, point));
}

TEST_CASE("two-points-poplygon returns false") {
  std::vector<std::array<int,2>> polygon;
  std::array<int,2> a{0,0};
  std::array<int,2> b{10,0};
  polygon.push_back(a);
  polygon.push_back(b);
  std::array<int,2> point{5, 1};
  CHECK(!geofence::isIn<int>(polygon, point));
}

TEST_CASE("three-points-poplygon and point inside returns true") {
  std::vector<std::array<int,2>> polygon;
  std::array<int,2> a{0,0};
  std::array<int,2> b{10,0};
  std::array<int,2> c{10,10};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  std::array<int,2> point{5, 1};
  CHECK(geofence::isIn<int>(polygon, point));
}

TEST_CASE("three-points-poplygon and point on edge returns true") {
  std::vector<std::array<int,2>> polygon;
  std::array<int,2> a{0,0};
  std::array<int,2> b{10,0};
  std::array<int,2> c{10,10};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  std::array<int,2> point{5, 0};
  CHECK(geofence::isIn<int>(polygon, point));
}

TEST_CASE("square and point inside returns true") {
  std::vector<std::array<int,2>> polygon;
  std::array<int,2> a{0,0};
  std::array<int,2> b{10,0};
  std::array<int,2> c{10,10};
  std::array<int,2> d{0,10};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  polygon.push_back(d);
  std::array<int,2> point{5, 5};
  CHECK(geofence::isIn<int>(polygon, point));
}

TEST_CASE("three-points-poplygon and point outside returns false") {
  std::vector<std::array<int,2>> polygon;
  std::array<int,2> a{0,0};
  std::array<int,2> b{10,0};
  std::array<int,2> c{10,10};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  std::array<int,2> point{5, 50};
  CHECK(!geofence::isIn<int>(polygon, point));
}

TEST_CASE("three-points-poplygon and float point outside returns false") {
  std::vector<std::array<float,2>> polygon;
  std::array<float,2> a{0,0};
  std::array<float,2> b{9.1,0};
  std::array<float,2> c{10.1,10.7};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  std::array<float,2> point{5.1, 50.5};
  CHECK(!geofence::isIn<float>(polygon, point));
}

TEST_CASE("three-points-poplygon and float point inside returns true") {
  std::vector<std::array<float,2>> polygon;
  std::array<float,2> a{0,0};
  std::array<float,2> b{10.1,0};
  std::array<float,2> c{10.2,10.3};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  std::array<float,2> point{5.2, 1.1};
  CHECK(geofence::isIn<float>(polygon, point));
}

TEST_CASE("WGS84 geofencing area - point outside") {
  std::vector<std::array<float,2>> polygon;
  std::array<float,2> a{57.725132, 11.916693};
  std::array<float,2> b{57.741855, 12.085297};
  std::array<float,2> c{57.746395, 12.214843};
  std::array<float,2> d{57.739790, 12.219870};
  std::array<float,2> e{57.730294, 12.089550};
  std::array<float,2> f{57.712741, 11.992101};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  polygon.push_back(d);
  polygon.push_back(e);
  polygon.push_back(f);
  std::array<float,2> point{57.675747, 12.135182};
  CHECK(!geofence::isIn<float>(polygon, point));
}

TEST_CASE("WGS84 geofencing area - point inside") {
  std::vector<std::array<float,2>> polygon;
  std::array<float,2> a{57.725132, 11.916693};
  std::array<float,2> b{57.741855, 12.085297};
  std::array<float,2> c{57.746395, 12.214843};
  std::array<float,2> d{57.739790, 12.219870};
  std::array<float,2> e{57.730294, 12.089550};
  std::array<float,2> f{57.712741, 11.992101};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  polygon.push_back(d);
  polygon.push_back(e);
  polygon.push_back(f);
  std::array<float,2> point{57.736694, 12.096124};
  CHECK(geofence::isIn<float>(polygon, point));
}

TEST_CASE("multiple tests without convex hull") {
  std::vector<std::array<uint8_t,2>> polygon;
  std::array<uint8_t,2> a{1, 3};
  std::array<uint8_t,2> b{6, 14};
  std::array<uint8_t,2> c{9, 1};
  std::array<uint8_t,2> d{17, 3};
  std::array<uint8_t,2> e{12, 10};
  std::array<uint8_t,2> f{5, 7};
  std::array<uint8_t,2> g{12, 5};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  polygon.push_back(d);
  polygon.push_back(e);
  polygon.push_back(f);
  polygon.push_back(g);

  {
    std::array<uint8_t,2> point{0, 0};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{100, 100};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{1, 3};
    CHECK(geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{4, 5};
    CHECK(geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{5, 7};
    CHECK(geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{6, 7};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{7, 7};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{8, 7};
    CHECK(geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{9, 5};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{10, 5};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{11, 5};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{12, 5};
    CHECK(geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{12, 10};
    CHECK(geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{13, 10};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{12, 11};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{17, 3};
    CHECK(geofence::isIn<uint8_t>(polygon, point));
  }
  {
    std::array<uint8_t,2> point{10, 10};
    CHECK(!geofence::isIn<uint8_t>(polygon, point));
  }
}

TEST_CASE("multiple tests with convex hull") {
  std::vector<std::array<uint8_t,2>> polygon;
  std::array<uint8_t,2> a{1, 3};
  std::array<uint8_t,2> b{6, 14};
  std::array<uint8_t,2> c{9, 1};
  std::array<uint8_t,2> d{17, 3};
  std::array<uint8_t,2> e{12, 10};
  std::array<uint8_t,2> f{5, 7};
  std::array<uint8_t,2> g{12, 5};
  polygon.push_back(a);
  polygon.push_back(b);
  polygon.push_back(c);
  polygon.push_back(d);
  polygon.push_back(e);
  polygon.push_back(f);
  polygon.push_back(g);

  auto convexHull = geofence::getConvexHull(polygon);
  std::cout << "convex hull is: " << std::endl;
  for(auto p : convexHull) {
    std::cout << "(" << +p[0] << "," << +p[1] << ")" << std::endl;
  }

  {
    std::array<uint8_t,2> point{0, 0};
    CHECK(!geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{100, 100};
    CHECK(!geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{1, 3};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{4, 5};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{5, 7};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{6, 7};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{7, 7};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{8, 7};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{9, 5};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{10, 5};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{11, 5};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{12, 5};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{12, 10};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{13, 10};
    CHECK(!geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{12, 11};
    CHECK(!geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{17, 3};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
  {
    std::array<uint8_t,2> point{10, 10};
    CHECK(geofence::isIn<uint8_t>(convexHull, point));
  }
} 

