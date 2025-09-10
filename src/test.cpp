#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cmath>

#include "../doctest.h"
// #include "../include/boid.hpp"
// #include "../include/graphics.hpp"
#include "../include/point.hpp"

// std::array<double, 3> distanceParams = flock::Flock::getDistancesParams();
// double d{distanceParams[0]};
// double prey_ds{distanceParams[1]};
// double predator_ds{distanceParams[2]};
// constexpr double preyMaxSpeed{5.};
// constexpr double predatorMaxSpeed{4.};

// constexpr double preyMinSpeed{3.};
// constexpr double predatorMinSpeed{2.5};

// ///////////////////////////////TESTING POINT CLASS
// ////////////////////////////////////////////////////////

TEST_CASE("testing Point class") {
  point::Point p0;
  point::Point p1(1., 2.);
  point::Point p2(-3., 5.);

  SUBCASE("testing getters") {
    CHECK(p0.getX() == 0.);
    CHECK(p0.getY() == 0.);
    CHECK(p1.getX() == 1.);
    CHECK(p1.getY() == 2.);
  }
  SUBCASE("testing distance methods") {
    CHECK(p0.distance() == doctest::Approx(0.00000000));
    CHECK(p2.distance() == doctest::Approx(5.83095189));
    CHECK(p0.distance(p1) == doctest::Approx(2.23606798));
    CHECK(p1.distance(p0) == doctest::Approx(2.23606798));
    CHECK(p1.distance(p2) == doctest::Approx(5.00000000));
    CHECK(p2.distance(p1) == doctest::Approx(5.00000000));
  }
  SUBCASE("testing toroidalDistance") {
    point::Point q1(330., 120.);
    point::Point q2(1010., 250.);
    point::Point q3(90., 560.);
    point::Point q4(940., 730.);
    CHECK(point::toroidalDistance(p0, q4) == doctest::Approx(465.295605));
    CHECK(point::toroidalDistance(q2, p0) == doctest::Approx(463.249393));
    CHECK(point::toroidalDistance(q1, q1) == doctest::Approx(0.00000000));
    CHECK(point::toroidalDistance(q1, q2) == doctest::Approx(692.314957));
    CHECK(point::toroidalDistance(q1, q3) == doctest::Approx(432.666153));
    CHECK(point::toroidalDistance(q1, q4) == doctest::Approx(638.905314));
    CHECK(point::toroidalDistance(q2, q3) == doctest::Approx(571.401785));
    CHECK(point::toroidalDistance(q2, q4) == doctest::Approx(327.566787));
    CHECK(point::toroidalDistance(q3, q4) == doctest::Approx(575.673519));
    CHECK(point::toroidalDistance(q4, q1) == doctest::Approx(638.905314));
  }
  SUBCASE("testing relativePosition") {
    point::Point q2(1010., 250.);
    point::Point q3(90., 560.);
    point::Point q4(940., 730.);
    CHECK(point::relativePosition(q2, q3).getX() == 480.);
    CHECK(point::relativePosition(q2, q3).getY() == 310.);
    CHECK(point::relativePosition(q2, q4).getX() == -70.);
    CHECK(point::relativePosition(q2, q4).getY() == -320.);
    CHECK(point::relativePosition(q3, q2).getX() == -480.);
    CHECK(point::relativePosition(q3, q2).getY() == -310.);
    CHECK(point::relativePosition(q3, q4).getX() == -550.);
    CHECK(point::relativePosition(q3, q4).getY() == 170.);
  }
  SUBCASE("testing operator +") {
    point::Point sum1 = p0 + p1;
    point::Point sum2 = p0 + p2;
    point::Point sum3 = p1 + p2;
    point::Point sum4 = p2 + p1;
    CHECK(sum1.getX() == doctest::Approx(1.00000000));
    CHECK(sum1.getY() == doctest::Approx(2.00000000));
    CHECK(sum2.getX() == doctest::Approx(-3.00000000));
    CHECK(sum2.getY() == doctest::Approx(5.00000000));
    CHECK(sum3.getX() == doctest::Approx(-2.00000000));
    CHECK(sum3.getY() == doctest::Approx(7.00000000));
    CHECK(sum4.getX() == doctest::Approx(-2.00000000));
    CHECK(sum4.getY() == doctest::Approx(7.00000000));
  }
  SUBCASE("testing operator -") {
    point::Point diff1 = p0 - p1;
    point::Point diff2 = p2 - p0;
    point::Point diff3 = p1 - p2;
    point::Point diff4 = p2 - p1;
    CHECK(diff1.getX() == doctest::Approx(-1.00000000));
    CHECK(diff1.getY() == doctest::Approx(-2.00000000));
    CHECK(diff2.getX() == doctest::Approx(-3.00000000));
    CHECK(diff2.getY() == doctest::Approx(+5.00000000));
    CHECK(diff3.getX() == doctest::Approx(+4.00000000));
    CHECK(diff3.getY() == doctest::Approx(-3.00000000));
    CHECK(diff4.getX() == doctest::Approx(-4.00000000));
    CHECK(diff4.getY() == doctest::Approx(+3.00000000));
  }
  SUBCASE("testing operator *") {
    double constant0{0.};
    double constant1{2.};
    double constant2{-3.};
    point::Point q00 = p0 * constant0;
    point::Point q01 = p0 * constant1;
    point::Point q02 = p0 * constant2;
    point::Point q20 = p2 * constant0;
    point::Point q21 = p2 * constant1;
    point::Point q22 = p2 * constant2;
    CHECK(q00.getX() == doctest::Approx(0.00000000));
    CHECK(q00.getY() == doctest::Approx(0.00000000));
    CHECK(q01.getX() == doctest::Approx(0.00000000));
    CHECK(q01.getY() == doctest::Approx(0.00000000));
    CHECK(q02.getX() == doctest::Approx(0.00000000));
    CHECK(q02.getY() == doctest::Approx(0.00000000));
    CHECK(q20.getX() == doctest::Approx(0.00000000));
    CHECK(q20.getY() == doctest::Approx(0.00000000));
    CHECK(q21.getX() == doctest::Approx(-6.00000000));
    CHECK(q21.getY() == doctest::Approx(10.00000000));
    CHECK(q22.getX() == doctest::Approx(9.00000000));
    CHECK(q22.getY() == doctest::Approx(-15.00000000));
  }
  SUBCASE("testing operator /") {
    double constant1{2.};
    double constant2{-3.};
    point::Point q11 = p1 / constant1;
    point::Point q12 = p1 / constant2;
    point::Point q21 = p2 / constant1;
    point::Point q22 = p2 / constant2;
    CHECK(q11.getX() == doctest::Approx(0.5));
    CHECK(q11.getY() == doctest::Approx(1.0));
    CHECK(q12.getX() == doctest::Approx(-1.0 / 3));
    CHECK(q12.getY() == doctest::Approx(-2.0 / 3));
    CHECK(q21.getX() == doctest::Approx(-1.5));
    CHECK(q21.getY() == doctest::Approx(2.5));
    CHECK(q22.getX() == doctest::Approx(1.0));
    CHECK(q22.getY() == doctest::Approx(-5.0 / 3));
  }
  SUBCASE("testing operator ==") {
    point::Point p3(1., 2.);
    CHECK((p1 == p3) == true);
    CHECK((p1 == p2) == false);
    CHECK((p2 == p3) == false);
  }
}

// ///////////////TESTING BOID CLASS//////////////////////////////

// const point::Point pos1(3., -2.);
// const point::Point vel1(5., -1.);

// const point::Point pos2 =
//     pos1 + 0.8 * d * 0.5 *
//                point::Point(vel1.getX() - sqrt(3) * vel1.getY(),
//                             sqrt(3) * vel1.getX() + vel1.getY()) /
//                vel1.distance();

// const point::Point vel2(-8.5, -6.);

// const point::Point pos3 =
//     pos1 + 0.8 * d * 0.5 *
//                point::Point(vel1.getX() + sqrt(3) * vel1.getY(),
//                             -sqrt(3) * vel1.getX() + vel1.getY()) /
//                / vel1.distance();
// const point::Point vel3(-7.3, 2.5);

// const point::Point pos4 =
//     pos1 + 0.5 * b_ds * 0.5 *
//                point::Point(vel1.getX() - sqrt(3) * vel1.getY(),
//                             sqrt(3) * vel1.getX() + vel1.getY()) /
//                vel1.distance();
// const point::Point vel4(-1., -2.);

// const point::Point pos5 =
//     pos1 + 0.5 * b_ds * 0.5 *
//                point::Point(vel1.getX() + sqrt(3) * vel1.getY(),
//                             -sqrt(3) * vel1.getX() + vel1.getY()) /
//                vel1.distance();
// const point::Point vel5(-5., -3.2);

// TEST_CASE("Testing Prey class") {
//   double s{0.1};
//   double a{0.1};
//   double c{0.004};
//   double r{s * 6};

//   const double x = pos1.getX();
//   const double y = pos1.getY();

//   boid::Prey b1(pos1, vel1);
//   boid::Prey b2(pos2, vel2);
//   boid::Prey b3(pos3, vel3);
//   boid::Prey b4(pos4, vel4);
//   boid::Prey b5;

//   b5.setBoid(pos5, vel5);

//   point::Point v1 = vel1;
//   point::Point v2 = vel2;
//   point::Point v3 = vel3;
//   point::Point v4 = vel4;
//   point::Point v5 = vel5;
// }

// std::vector<std::shared_ptr<boid::Boid>> near_b1{
//     std::make_shared<boid::Prey>(b2), std::make_shared<boid::Prey>(b3),
//     std::make_shared<boid::Prey>(b4), std::make_shared<boid::Prey>(b5)};

// SUBCASE("Testing getters") {
// boid:
//   Prey b0;
//   CHECK(b0.getPosition() == point::Point(0., 0.));
//   CHECK(b0.getVelocity() == point::Point(0., 0.));

//   CHECK(b1.getPosition() == point::Point(3., -2.));
//   CHECK(b1.getVelocity() == point::Point(5., -1.));
// }

// SUBCASE("Testing setters") {
//   CHECK(b5.getPosition().getX() == doctest::Approx(pos5.getX()));
//   CHECK(b5.getPosition().getY() == doctest::Approx(pos5.getY()));

//   CHECK(b5.getVelocity() == point::Point(-5., -3.2));
// }

// SUBCASE("Testing separation method") {
//   double sep1_x = -s * 0.5 * b_ds * vel1.getX() / vel1.distance();
//   double sep1_y = -s * 0.5 * b_ds * vel1.getY() / vel1.distance();

//   CHECK(b1.separation(s, b_ds, near_b1).getX() == doctest::Approx(sep1_x));
//   CHECK(b1.separation(s, b_ds, near_b1).getY() == doctest::Approx(sep1_y));
// }

// SUBCASE("Testing cohesion method") {
//   double coh1_x = c * (0.5 * b_ds + 0.8 * d) * vel1.getX() /
//   vel1.distance(); double coh1_y = c * (0.5 * b_ds + 0.8 * d) * vel1.getY()
//   / vel1.distance();

//   CHECK(b1.cohesion(c, near_b1).getX() == doctest::Approx(coh1_x));
//   CHECK(b1.cohesion(c, near_b1).getY() == doctest::Approx(coh1_y));
// }

// SUBCASE("Testing alignment method") {
//   double al1_x =
//       a * ((vel2.getX() + vel3.getX() + vel4.getX() + vel5.getX()) / 4.
//       - 5.);
//   double al1_y =
//       a * ((vel2.getY() + vel3.getY() + vel4.getY() + vel5.getY()) / 4.
//       + 1.);

//   CHECK(b1.alignment(a, near_b1).getX() == doctest::Approx(al1_x));
//   CHECK(b1.alignment(a, near_b1).getY() == doctest::Approx(al1_y));
// }

// SUBCASE("Testing repulsion method") {
//   // here we apply boid::Prey::repulsion on near_b1, which identifies the
//   prey
//   // near b1, even if it was initially intended to be applied on
//   near_predators.

//   double rep1_x = -r * (0.5 * b_ds + 0.8 * d) * vel1.getX() /
//   vel1.distance(); double rep1_y = -r * (0.5 * b_ds + 0.8 * d) * vel1.getY()
//   / vel1.distance();

//   CHECK(b1.repulsion(r, near_b1).getX() == doctest::Approx(rep1_x));
//   CHECK(b1.repulsion(r, near_b1).getY() == doctest::Approx(rep1_y));
// }

// SUBCASE("Testing clamp method") {
//   b1.clamp(preyMinSpeed, preyMaxSpeed, v1);
//   b2.clamp(preyMinSpeed, preyMaxSpeed, v2);
//   b3.clamp(preyMinSpeed, preyMaxSpeed, v3);
//   b4.clamp(preyMinSpeed, preyMaxSpeed, v4);
//   b5.clamp(preyMinSpeed, preyMaxSpeed, v5);

//   CHECK(v1.getX() ==
//         doctest::Approx(preyMaxSpeed * vel1.getX() / vel1.distance()));
//   CHECK(v1.getY() ==
//         doctest::Approx(preyMaxSpeed * vel1.getY() / vel1.distance()));

//   CHECK(v2.getX() ==
//         doctest::Approx(preyMaxSpeed * vel2.getX() / vel2.distance()));
//   CHECK(v2.getY() ==
//         doctest::Approx(preyMaxSpeed * vel2.getY() / vel2.distance()));

//   CHECK(v3.getX() ==
//         doctest::Approx(preyMaxSpeed * vel3.getX() / vel3.distance()));
//   CHECK(v3.getY() ==
//         doctest::Approx(preyMaxSpeed * vel3.getY() / vel3.distance()));

//   CHECK(v4.getX() ==
//         doctest::Approx(preyMinSpeed * ve4.getX() / vel4.distance()));
//   CHECK(v4.getY() ==
//         doctest::Approx(preyMinSpeed * ve4.getY() / vel4.distance()));

//   CHECK(v5.getX() ==
//         doctest::Approx(preyMaxSpeed * vel5.getX() / vel5.distance()));
//   CHECK(v5.getY() ==
//         doctest::Approx(preyMaxSpeed * vel5.getY() / vel5.distance()));
// }

// TEST_CASE("Testing Predator class") {
//   double ch{0.004};

//   boid::Predator p1(pos1, vel1);
//   boid::Predator p2(pos2, vel2);
//   boid::Predator p3(pos3, vel3);
//   boid::Predator p4;

//   const point::Point pos6 =
//       pos1 + 0.5 * d * 0.5 *
//                  point::Point(vel1.getX() - sqrt(3) * vel1.getY(),
//                               sqrt(3) * vel1.getX() + vel1.getY()) /
//                  vel1.distance();
//   p4.setBoid(pos6, vel4);

//   point::Point v1 = vel1;
//   point::Point v2 = vel2;
//   point::Point v3 = vel3;
//   point::Point v4 = vel4;

//   std::vector<std::shared_ptr<boid::Boid>> near_p1{
//       std::make_shared<boid::Predator>(p2),
//       std::make_shared<boid::Predator>(p3),
//       std::make_shared<boid::Predator>(p4)};

//   SUBCASE("Testing getters") {
//     boid::Predator p0;
//     CHECK(p0.getPosition() == point::Point(0., 0.));
//     CHECK(p0.getVelocity() == point::Point(0., 0.));
//     CHECK(p1.getPosition() == point::Point(3., -2.));
//     CHECK(p1.getVelocity() == point::Point(5., -1.));
//   }

//   SUBCASE("Testing setters") {
//     CHECK(p2.getPosition().getX() == doctest::Approx(pos2.getX()));
//     CHECK(p2.getPosition().getY() == doctest::Approx(pos2.getY()));
//     CHECK(p2.getVelocity() == point::Point(-8.5, -6.));

//     CHECK(p3.getPosition().getX() == doctest::Approx(pos3.getX()));
//     CHECK(p3.getPosition().getY() == doctest::Approx(pos3.getY()));
//     CHECK(p3.getVelocity() == point::Point(-7.3, 2.5));

//     CHECK(p4.getPosition().getX() == doctest::Approx(pos6.getX()));
//     CHECK(p4.getPosition().getY() == doctest::Approx(pos6.getY()));
//     CHECK(p4.getVelocity() == point::Point(-1., -2.));
//   }

//   SUBCASE("Testing chase method") {
//     // here we apply boidd::Predator::chase on near_p1, which identifies the
//     // predators near p1, even if it was initially intended to be applied on
//     // near_boids.

//     double chase1_x =
//         ch * d / 3. * (1.3 * std::sin(alfa) + 0.8 * std::sin(beta));
//     double chase1_y =
//         ch * -d / 3. * (1.3 * std::cos(alfa) + 0.8 * std::cos(beta));

//     CHECK(p1.chase(ch, near_p1).getX() == doctest::Approx(chase1_x));
//     CHECK(p1.chase(ch, near_p1).getY() == doctest::Approx(chase1_y));
//   }

//   SUBCASE("Testing clamp method") {
//     p1.clamp(predatorMinSpeed, predatorMaxSpeed, v1);
//     p2.clamp(predatorMinSpeed, predatorMaxSpeed, v2);
//     p3.clamp(predatorMinSpeed, predatorMaxSpeed, v3);
//     p4.clamp(predatorMinSpeed, predatorMaxSpeed, v4);

//     CHECK(v1.getX() ==
//           doctest::Approx(predatorMaxSpeed * vel1.getX() /
//           vel1.distance()));
//     CHECK(v1.getY() ==
//           doctest::Approx(predatorMaxSpeed * vel1.getY() /
//           vel1.distance()));

//     CHECK(v2.getX() ==
//           doctest::Approx(predatorMaxSpeed * vel2.getX() /
//           vel2.distance()));
//     CHECK(v2.getY() ==
//           doctest::Approx(predatorMaxSpeed * vel2.getY() /
//           vel2.distance()));

//     CHECK(v3.getX() ==
//           doctest::Approx(predatorMaxSpeed * vel3.getX() /
//           vel3.distance()));
//     CHECK(v3.getY() ==
//           doctest::Approx(predatorMaxSpeed * vel3.getY() /
//           vel3.distance()));

//     CHECK(v4.getX() ==
//           doctest::Approx(predatorMinSpeed * ve4.getX() / vel4.distance()));
//     CHECK(v4.getY() ==
//           doctest::Approx(predatorMinSpeed * ve4.getY() / vel4.distance()));
//   }
// }
