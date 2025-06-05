#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cmath>

#include "../doctest.h"
#include "../include/boid.hpp"
#include "../include/constants.hpp"
#include "../include/point.hpp"

TEST_CASE("testing Point class") {
  point::Point p0;
  point::Point p1(1., 2.);
  point::Point p2(-3., 5.);

  SUBCASE("testing getters") {
    CHECK(p0.get_x() == 0.);
    CHECK(p0.get_y() == 0.);
    CHECK(p1.get_x() == 1.);
    CHECK(p1.get_y() == 2.);
  }
  SUBCASE("testing distance methods") {
    CHECK(p0.distance() == doctest::Approx(0.00000000));
    CHECK(p1.distance() == doctest::Approx(2.23606798));
    CHECK(p2.distance() == doctest::Approx(5.83095189));
    CHECK(p0.distance(p1) == doctest::Approx(2.23606798));
    CHECK(p1.distance(p0) == doctest::Approx(2.23606798));
    CHECK(p1.distance(p2) == doctest::Approx(5.00000000));
    CHECK(p2.distance(p1) == doctest::Approx(5.00000000));
    CHECK(p0.distance(p2) == doctest::Approx(5.83095189));
    CHECK(p2.distance(p0) == doctest::Approx(5.83095189));
  }
  SUBCASE("testing toroidal distance") {
    point::Point q1(330., 120.);
    point::Point q2(1010., 250.);
    point::Point q3(90., 560.);
    point::Point q4(940., 730.);

    CHECK(point::toroidal_distance(p0) == doctest::Approx(0.00000000));
    CHECK(point::toroidal_distance(q1) == doctest::Approx(351.140997));
    CHECK(point::toroidal_distance(q2) == doctest::Approx(314.006369));
    CHECK(point::toroidal_distance(q3) == doctest::Approx(256.320112));
    CHECK(point::toroidal_distance(q4) == doctest::Approx(269.258240));

    CHECK(point::toroidal_distance(p0, p0) == doctest::Approx(0.00000000));
    CHECK(point::toroidal_distance(p0, q1) == doctest::Approx(351.140997));
    CHECK(point::toroidal_distance(p0, q2) == doctest::Approx(314.006369));
    CHECK(point::toroidal_distance(p0, q3) == doctest::Approx(256.320112));
    CHECK(point::toroidal_distance(p0, q4) == doctest::Approx(269.258240));
    CHECK(point::toroidal_distance(q1, p0) == doctest::Approx(351.140997));
    CHECK(point::toroidal_distance(q1, q1) == doctest::Approx(0.00000000));
    CHECK(point::toroidal_distance(q1, q2) == doctest::Approx(536.003731));
    CHECK(point::toroidal_distance(q1, q3) == doctest::Approx(432.666153));
    CHECK(point::toroidal_distance(q1, q4) == doctest::Approx(619.838689));
    CHECK(point::toroidal_distance(q2, p0) == doctest::Approx(314.006369));
    CHECK(point::toroidal_distance(q2, q1) == doctest::Approx(536.003731));
    CHECK(point::toroidal_distance(q2, q2) == doctest::Approx(0.00000000));
    CHECK(point::toroidal_distance(q2, q3) == doctest::Approx(417.731971));
    CHECK(point::toroidal_distance(q2, q4) == doctest::Approx(327.566787));
    CHECK(point::toroidal_distance(q3, p0) == doctest::Approx(256.320112));
    CHECK(point::toroidal_distance(q3, q1) == doctest::Approx(432.666153));
    CHECK(point::toroidal_distance(q3, q2) == doctest::Approx(417.731971));
    CHECK(point::toroidal_distance(q3, q3) == doctest::Approx(0.00000000));
    CHECK(point::toroidal_distance(q3, q4) == doctest::Approx(389.101529));
    CHECK(point::toroidal_distance(q4, p0) == doctest::Approx(269.258240));
    CHECK(point::toroidal_distance(q4, q1) == doctest::Approx(619.838689));
    CHECK(point::toroidal_distance(q4, q2) == doctest::Approx(327.566787));
    CHECK(point::toroidal_distance(q4, q3) == doctest::Approx(389.101529));
    CHECK(point::toroidal_distance(q4, q4) == doctest::Approx(0.00000000));
  }

  SUBCASE("testing operator +") {
    point::Point sum1 = p0 + p1;
    point::Point sum2 = p1 + p0;
    point::Point sum3 = p0 + p2;
    point::Point sum4 = p2 + p0;
    point::Point sum5 = p1 + p2;
    point::Point sum6 = p2 + p1;
    CHECK(sum1.get_x() == doctest::Approx(1.00000000));
    CHECK(sum1.get_y() == doctest::Approx(2.00000000));
    CHECK(sum2.get_x() == doctest::Approx(1.00000000));
    CHECK(sum2.get_y() == doctest::Approx(2.00000000));
    CHECK(sum3.get_x() == doctest::Approx(-3.00000000));
    CHECK(sum3.get_y() == doctest::Approx(5.00000000));
    CHECK(sum4.get_x() == doctest::Approx(-3.00000000));
    CHECK(sum4.get_y() == doctest::Approx(5.00000000));
    CHECK(sum5.get_x() == doctest::Approx(-2.00000000));
    CHECK(sum5.get_y() == doctest::Approx(7.00000000));
    CHECK(sum6.get_x() == doctest::Approx(-2.00000000));
    CHECK(sum6.get_y() == doctest::Approx(7.00000000));
  }

  SUBCASE("testing operator -") {
    point::Point diff1 = p0 - p1;
    point::Point diff2 = p1 - p0;
    point::Point diff3 = p2 - p0;
    point::Point diff4 = p0 - p2;
    point::Point diff5 = p1 - p2;
    point::Point diff6 = p2 - p1;
    CHECK(diff1.get_x() == doctest::Approx(-1.00000000));
    CHECK(diff1.get_y() == doctest::Approx(-2.00000000));
    CHECK(diff2.get_x() == doctest::Approx(1.00000000));
    CHECK(diff2.get_y() == doctest::Approx(2.00000000));
    CHECK(diff3.get_x() == doctest::Approx(-3.00000000));
    CHECK(diff3.get_y() == doctest::Approx(+5.00000000));
    CHECK(diff4.get_x() == doctest::Approx(+3.00000000));
    CHECK(diff4.get_y() == doctest::Approx(-5.00000000));
    CHECK(diff5.get_x() == doctest::Approx(4.00000000));
    CHECK(diff5.get_y() == doctest::Approx(-3.00000000));
    CHECK(diff6.get_x() == doctest::Approx(-4.00000000));
    CHECK(diff6.get_y() == doctest::Approx(+3.00000000));
  }
  SUBCASE("testing operator *") {
    double constant0{0.};
    double constant1{2.};
    double constant2{-3.};
    point::Point q00 = p0 * constant0;
    point::Point q01 = p0 * constant1;
    point::Point q02 = p0 * constant2;
    point::Point q10 = p1 * constant0;
    point::Point q11 = p1 * constant1;
    point::Point q12 = p1 * constant2;
    point::Point q20 = p2 * constant0;
    point::Point q21 = p2 * constant1;
    point::Point q22 = p2 * constant2;
    CHECK(q00.get_x() == doctest::Approx(0.00000000));
    CHECK(q00.get_y() == doctest::Approx(0.00000000));
    CHECK(q01.get_x() == doctest::Approx(0.00000000));
    CHECK(q01.get_y() == doctest::Approx(0.00000000));
    CHECK(q02.get_x() == doctest::Approx(0.00000000));
    CHECK(q02.get_y() == doctest::Approx(0.00000000));
    CHECK(q10.get_x() == doctest::Approx(0.00000000));
    CHECK(q10.get_y() == doctest::Approx(0.00000000));
    CHECK(q11.get_x() == doctest::Approx(2.00000000));
    CHECK(q11.get_y() == doctest::Approx(4.00000000));
    CHECK(q12.get_x() == doctest::Approx(-3.0000000));
    CHECK(q12.get_y() == doctest::Approx(-6.0000000));
    CHECK(q20.get_x() == doctest::Approx(0.00000000));
    CHECK(q20.get_y() == doctest::Approx(0.00000000));
    CHECK(q21.get_x() == doctest::Approx(-6.00000000));
    CHECK(q21.get_y() == doctest::Approx(10.00000000));
    CHECK(q22.get_x() == doctest::Approx(9.00000000));
    CHECK(q22.get_y() == doctest::Approx(-15.00000000));
  }
}

TEST_CASE("testing Boid class") {}
