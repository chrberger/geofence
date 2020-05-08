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

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "geofence.hpp"

#include <string>

TEST_CASE("non-poplygon returns false") {
    std::vector<std::array<int,2>> polygon;
    std::array<int,2> point{1, 2};
    REQUIRE(!geofence::isIn<int>(polygon, point));
}

TEST_CASE("one-point-poplygon returns false") {
    std::vector<std::array<int,2>> polygon;
    std::array<int,2> a{0,0};
    polygon.push_back(a);
    std::array<int,2> point{1, 2};
    REQUIRE(!geofence::isIn<int>(polygon, point));
}

TEST_CASE("two-points-poplygon returns false") {
    std::vector<std::array<int,2>> polygon;
    std::array<int,2> a{0,0};
    std::array<int,2> b{10,0};
    polygon.push_back(a);
    polygon.push_back(b);
    std::array<int,2> point{5, 1};
    REQUIRE(!geofence::isIn<int>(polygon, point));
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
    REQUIRE(geofence::isIn<int>(polygon, point));
}

TEST_CASE("three-points-poplygon and point on side returns false") {
    std::vector<std::array<int,2>> polygon;
    std::array<int,2> a{0,0};
    std::array<int,2> b{10,0};
    std::array<int,2> c{10,10};
    polygon.push_back(a);
    polygon.push_back(b);
    polygon.push_back(c);
    std::array<int,2> point{5, 0};
    REQUIRE(geofence::isIn<int>(polygon, point));
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
    REQUIRE(!geofence::isIn<int>(polygon, point));
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
    REQUIRE(!geofence::isIn<float>(polygon, point));
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
    REQUIRE(geofence::isIn<float>(polygon, point));
}

