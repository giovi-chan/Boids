#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cmath>

#include "../doctest.h"
#include "../include/boid.hpp"
#include "../include/point.hpp"

TEST_CASE("testing Point class") {
  point::Point p0;
  point::Point p1(1., 2.);
  point::Point p2(-3., 5.);
  double constant0{0.};
  double constant1{2.};
  double constant2{-3.};

  SUBCASE("testing getters") {
    CHECK(p0.x() == 0.);
    CHECK(p0.y() == 0.);
    CHECK(p1.x() == 1.);
    CHECK(p1.y() == 2.);
  }
  SUBCASE("testing distance") {
    CHECK(p0.distance() == doctest::Approx(0.00000000));
    CHECK(p1.distance() == doctest::Approx(2.23606798));
    CHECK(p2.distance() == doctest::Approx(5.83095189));
    CHECK(p0.distance(p1) == doctest::Approx(2.23606798));
    CHECK(p1.distance(p0) == doctest::Approx(2.23606798));
    CHECK(p1.distance(p2) == doctest::Approx(8.06225775));
    CHECK(p2.distance(p1) == doctest::Approx(8.06225775));
    CHECK(p0.distance(p2) == doctest::Approx(5.83095189));
    CHECK(p2.distance(p0) == doctest::Approx(5.83095189));
  }
  SUBCASE("testing operator +") {
    point::Point sum1 = p0 + p1;
    point::Point sum2 = p1 + p0;
    point::Point sum3 = p0 + p2;
    point::Point sum4 = p2 + p0;
    point::Point sum5 = p1 + p2;
    point::Point sum6 = p2 + p1;
    CHECK(sum1.x() == doctest::Approx(1.00000000));
    CHECK(sum1.y() == doctest::Approx(2.00000000));
    CHECK(sum2.x() == doctest::Approx(1.00000000));
    CHECK(sum2.y() == doctest::Approx(2.00000000));
    CHECK(sum3.x() == doctest::Approx(-3.00000000));
    CHECK(sum3.y() == doctest::Approx(5.00000000));
    CHECK(sum4.x() == doctest::Approx(-3.00000000));
    CHECK(sum4.y() == doctest::Approx(5.00000000));
    CHECK(sum5.x() == doctest::Approx(-2.00000000));
    CHECK(sum5.y() == doctest::Approx(3.00000000));
    CHECK(sum6.x() == doctest::Approx(-2.00000000));
    CHECK(sum6.y() == doctest::Approx(3.00000000));
  }

  SUBCASE("testing operator -") {
    point::Point diff1 = p0 - p1;
    point::Point diff2 = p1 - p0;
    point::Point diff3 = p2 - p0;
    point::Point diff4 = p0 - p2;
    point::Point diff5 = p1 - p2;
    point::Point diff6 = p2 - p1;
    CHECK(diff1.x() == doctest::Approx(-1.00000000));
    CHECK(diff1.y() == doctest::Approx(-2.00000000));
    CHECK(diff2.x() == doctest::Approx(1.00000000));
    CHECK(diff2.y() == doctest::Approx(2.00000000));
    CHECK(diff3.x() == doctest::Approx(-3.00000000));
    CHECK(diff3.y() == doctest::Approx(+5.00000000));
    CHECK(diff4.x() == doctest::Approx(+3.00000000));
    CHECK(diff4.y() == doctest::Approx(-5.00000000));
    CHECK(diff5.x() == doctest::Approx(4.00000000));
    CHECK(diff5.y() == doctest::Approx(-3.00000000));
    CHECK(diff6.x() == doctest::Approx(-4.00000000));
    CHECK(diff6.y() == doctest::Approx(+3.00000000));
  }
  SUBCASE("testing operator *") {
    point::Point q00 = p0 * constant0;
    point::Point q01 = p0 * constant1;
    point::Point q02 = p0 * constant2;
    point::Point q10 = p1 * constant0;
    point::Point q11 = p1 * constant1;
    point::Point q12 = p1 * constant2;
    point::Point q20 = p2 * constant0;
    point::Point q21 = p2 * constant1;
    point::Point q22 = p2 * constant2;
    CHECK(q00.x() == doctest::Approx(0.00000000));
    CHECK(q00.y() == doctest::Approx(0.00000000));
    CHECK(q01.x() == doctest::Approx(0.00000000));
    CHECK(q01.y() == doctest::Approx(0.00000000));
    CHECK(q02.x() == doctest::Approx(0.00000000));
    CHECK(q02.x() == doctest::Approx(0.00000000));
    CHECK(q10.x() == doctest::Approx(0.00000000));
    CHECK(q10.x() == doctest::Approx(0.00000000));
    CHECK(q11.x() == doctest::Approx(2.00000000));
    CHECK(q11.x() == doctest::Approx(4.00000000));
    CHECK(q12.x() == doctest::Approx(-3.0000000));
    CHECK(q12.x() == doctest::Approx(-6.0000000));
    CHECK(q20.x() == doctest::Approx(0.00000000));
    CHECK(q20.x() == doctest::Approx(0.00000000));
    CHECK(q21.x() == doctest::Approx(-6.00000000));
    CHECK(q21.x() == doctest::Approx(10.00000000));
    CHECK(q22.x() == doctest::Approx(9.00000000));
    CHECK(q22.x() == doctest::Approx(-15.00000000));
  }
}

TEST_CASE("testing Boid class") {}
