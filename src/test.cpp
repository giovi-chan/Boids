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

  SUBCASE("testing relative_position") {
    point::Point q1(330., 120.);
    point::Point q2(1010., 250.);
    point::Point q3(90., 560.);
    point::Point q4(940., 730.);
    CHECK(point::relative_position(q1, q2).get_x() == -520.);
    CHECK(point::relative_position(q1, q2).get_y() == 130.);
    CHECK(point::relative_position(q1, q3).get_x() == -240.);
    CHECK(point::relative_position(q1, q3).get_y() == -360.);
    CHECK(point::relative_position(q1, q4).get_x() == -590.);
    CHECK(point::relative_position(q1, q4).get_y() == -190.);
    CHECK(point::relative_position(q2, q1).get_x() == 520.);
    CHECK(point::relative_position(q2, q1).get_y() == -130.);
    CHECK(point::relative_position(q2, q3).get_x() == 280.);
    CHECK(point::relative_position(q2, q3).get_y() == 310.);
    CHECK(point::relative_position(q2, q4).get_x() == -70.);
    CHECK(point::relative_position(q2, q4).get_y() == -320.);
    CHECK(point::relative_position(q3, q1).get_x() == 240.);
    CHECK(point::relative_position(q3, q1).get_y() == 360.);
    CHECK(point::relative_position(q3, q2).get_x() == -280.);
    CHECK(point::relative_position(q3, q2).get_y() == -310.);
    CHECK(point::relative_position(q3, q4).get_x() == -350.);
    CHECK(point::relative_position(q3, q4).get_y() == 170.);
    CHECK(point::relative_position(q4, q1).get_x() == 590.);
    CHECK(point::relative_position(q4, q1).get_y() == 190.);
    CHECK(point::relative_position(q4, q2).get_x() == +70);
    CHECK(point::relative_position(q4, q2).get_y() == 320.);
    CHECK(point::relative_position(q4, q3).get_x() == +350.);
    CHECK(point::relative_position(q4, q3).get_y() == -170.);
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

  SUBCASE("testing operator /") {
    double constant1{2.};
    double constant2{-3.};
    point::Point q11 = p1 / constant1;
    point::Point q12 = p1 / constant2;
    point::Point q21 = p2 / constant1;
    point::Point q22 = p2 / constant2;

    CHECK(q11.get_x() == doctest::Approx(0.5));
    CHECK(q11.get_y() == doctest::Approx(1.0));
    CHECK(q12.get_x() == doctest::Approx(-1.0 / 3));
    CHECK(q12.get_y() == doctest::Approx(-2.0 / 3));
    CHECK(q21.get_x() == doctest::Approx(-1.5));
    CHECK(q21.get_y() == doctest::Approx(2.5));
    CHECK(q22.get_x() == doctest::Approx(1.0));
    CHECK(q22.get_y() == doctest::Approx(-5.0 / 3));
  }
  SUBCASE("testing operator ==") {
    point::Point p3(1., 2.);
    CHECK((p1 == p3) == true);
    CHECK((p1 == p2) == false);
    CHECK((p2 == p3) == false);
  }
}

TEST_CASE("testing Boid class") {
  SUBCASE("testing Boid constructor, getters and setters") {
    point::Point initial_position(1.0, 2.0);
    point::Point initial_velocity(0.5, -1.5);

    boid::Boid b(initial_position, initial_velocity);

    SUBCASE("Constructor initializes correctly") {
      CHECK(b.get_position().get_x() == doctest::Approx(1.0));
      CHECK(b.get_position().get_y() == doctest::Approx(2.0));
      CHECK(b.get_velocity().get_x() == doctest::Approx(0.5));
      CHECK(b.get_velocity().get_y() == doctest::Approx(-1.5));
    }

    SUBCASE("Setters update state correctly") {
      point::Point new_position(3.0, 4.0);
      point::Point new_velocity(-0.5, 2.5);

      b.set_position(new_position);
      b.set_velocity(new_velocity);

      CHECK(b.get_position().get_x() == doctest::Approx(3.0));
      CHECK(b.get_position().get_y() == doctest::Approx(4.0));
      CHECK(b.get_velocity().get_x() == doctest::Approx(-0.5));
      CHECK(b.get_velocity().get_y() == doctest::Approx(2.5));
    }
  }

  SUBCASE("testing angle ") {
    boid::Boid b0(point::Point(1., 1.), point::Point(0., 1.));
    boid::Boid b1(point::Point(1., 7.), point::Point(4., 0.));
    boid::Boid b2(point::Point(2., sqrt(3) + 1), point::Point(0., 1.));
    CHECK(b0.angle(b0) == doctest::Approx(0.));
    CHECK(b0.angle(b1) == doctest::Approx(0.));
    CHECK(b0.angle(b2) == doctest::Approx(0.523599));
  }

  SUBCASE("testing alignment with null alignment coefficient") {
    SUBCASE("Alignment with no neighbors") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors;
      point::Point result = b.alignment(neighbors, 0.);
      CHECK(result == point::Point{0., 0.});
    }

    SUBCASE("Alignment with one neighbor - same velocity") {
      boid::Boid b(point::Point{0., 0.}, point::Point{2., 2.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{1., 1.}, point::Point{2., 2.})};
      point::Point result = b.alignment(neighbors, 0.);
      CHECK(result == point::Point{0., 0.});
    }

    SUBCASE("Alignment with one neighbor - different velocity") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{1., 1.}, point::Point{3., 3.})};
      point::Point result = b.alignment(neighbors, 0.);
      CHECK(result == point::Point{0., 0.});
    }
  }

  SUBCASE("testing alignment with non-null alignment coefficient") {
    double a{2.};
    SUBCASE("Alignment with no neighbors") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors;
      point::Point result = b.alignment(neighbors, a);
      CHECK(result == point::Point{0., 0.});
    }

    SUBCASE("Alignment with one neighbor - same velocity") {
      boid::Boid b(point::Point{0., 0.}, point::Point{2., 2.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{1., 1.}, point::Point{2., 2.})};
      point::Point result = b.alignment(neighbors, a);
      CHECK(result == point::Point{0., 0.});
    }

    SUBCASE("Alignment with one neighbor - different velocity") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{1., 1.}, point::Point{3., 3.})};
      point::Point result = b.alignment(neighbors, a);
      CHECK(result == point::Point{4., 4.});
    }

    SUBCASE("Alignment with multiple neighbors - average velocity") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{1., 1.}, point::Point{3., 3.}),
          boid::Boid(point::Point{2., 2.}, point::Point{1., 5.}),
          boid::Boid(point::Point{3., 3.}, point::Point{2., 2.})};
      point::Point result = b.alignment(neighbors, a);

      CHECK(result.get_x() == doctest::Approx(2.));
      CHECK(result.get_y() == doctest::Approx(4.6666667));
    }
  }

  SUBCASE("testing separation") {
    double separation_dist = 5.0;
    double separation_coeff = 0.5;

    SUBCASE("Separation with no neighbors") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors;
      point::Point result =
          b.separation(neighbors, separation_dist, separation_coeff);
      CHECK(result == point::Point{0., 0.});
    }

    SUBCASE("Separation with neighbors all outside separation distance") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{10., 0.}, point::Point{0., 0.}),
          boid::Boid(point::Point{0., -10.}, point::Point{0., 0.})};
      point::Point result =
          b.separation(neighbors, separation_dist, separation_coeff);
      CHECK(result == point::Point{0., 0.});
    }

    SUBCASE("Separation with one neighbor within range") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      boid::Boid n1(point::Point{3., 0.},
                    point::Point{0., 0.});  // distanza 3.0
      std::vector<boid::Boid> neighbors = {n1};
      point::Point result =
          b.separation(neighbors, separation_dist, separation_coeff);
      CHECK(result.get_x() == doctest::Approx(-1.5));
      CHECK(result.get_y() == doctest::Approx(0));
    }

    SUBCASE("Separation with multiple neighbors, mixed range") {
      boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{1., 0.}, point::Point{0., 0.}),
          boid::Boid(point::Point{-2., 0.}, point::Point{0., 0.}),
          boid::Boid(point::Point{10., 10.}, point::Point{0., 0.})};

      point::Point result =
          b.separation(neighbors, separation_dist, separation_coeff);
      CHECK(result.get_x() == doctest::Approx(0.5));
      CHECK(result.get_y() == doctest::Approx(0.));
    }

    SUBCASE("Separation with all neighbors on top of the boid (distance 0)") {
      boid::Boid b(point::Point{1., 1.}, point::Point{0., 0.});
      std::vector<boid::Boid> neighbors = {
          boid::Boid(point::Point{1., 1.}, point::Point{0., 0.}),
          boid::Boid(point::Point{1., 1.}, point::Point{0., 0.})};

      point::Point result =
          b.separation(neighbors, separation_dist, separation_coeff);
      CHECK(result == point::Point{0., 0.});
    }

    SUBCASE("testing cohesion") {
      double cohesion_coeff = 0.5;

      SUBCASE("Cohesion with no neighbors") {
        boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
        std::vector<boid::Boid> neighbors;
        point::Point result = b.cohesion(neighbors, cohesion_coeff);
        CHECK(result == point::Point{0., 0.});
      }

      SUBCASE("Cohesion with one neighbor") {
        boid::Boid b(point::Point{0., 0.}, point::Point{1., 1.});
        boid::Boid n1(point::Point{4., 0.}, point::Point{0., 0.});
        std::vector<boid::Boid> neighbors = {n1};
        point::Point result = b.cohesion(neighbors, cohesion_coeff);
        // center of mass is (4, 0), relative to b is (4, 0)
        CHECK(result.get_x() == doctest::Approx(2.0));
        CHECK(result.get_y() == doctest::Approx(0.0));
      }

      SUBCASE("Cohesion with multiple neighbors") {
        boid::Boid b(point::Point{1., 1.}, point::Point{1., 1.});
        std::vector<boid::Boid> neighbors = {
            boid::Boid(point::Point{3., 1.}, point::Point{0., 0.}),
            boid::Boid(point::Point{1., 3.}, point::Point{0., 0.}),
            boid::Boid(point::Point{-1., 1.}, point::Point{0., 0.})};

        // center of mass = (3 + 1 + (-1)) / 3 = (1, 5/3)
        // relative to b (1,1): (0, 2/3)
        // scaled by coeff: (0, 1/3)
        point::Point result = b.cohesion(neighbors, cohesion_coeff);
        CHECK(result.get_x() == doctest::Approx(0.0));
        CHECK(result.get_y() == doctest::Approx(1.0 / 3.0));
      }

      SUBCASE("Cohesion with neighbors all at same location as boid") {
        boid::Boid b(point::Point{2., 2.}, point::Point{0., 0.});
        std::vector<boid::Boid> neighbors = {
            boid::Boid(point::Point{2., 2.}, point::Point{0., 0.}),
            boid::Boid(point::Point{2., 2.}, point::Point{0., 0.})};

        point::Point result = b.cohesion(neighbors, cohesion_coeff);
        // Relative positions will all be (0,0), so center of mass is (0,0)
        CHECK(result == point::Point{0., 0.});
      }
    }
  }
}
