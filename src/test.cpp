#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cmath>

#include "../doctest.h"
#include "../include/boid.hpp"
#include "../include/flock.hpp"
#include "../include/graphics.hpp"
#include "../include/point.hpp"

const std::array<double, 3> distance_parameters =
    flock::Flock::getDistanceParameters();
const double d_{distance_parameters[0]};
const double prey_ds_{distance_parameters[1]};
const double predator_ds_{distance_parameters[2]};

constexpr double prey_max_speed{5.};
constexpr double predator_max_speed{4.};
constexpr double prey_min_speed{3.};
constexpr double predator_min_speed{2.5};

//////////////////TESTING POINT CLASS///////////////////////////////////

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

  SUBCASE("testing setters") {
    p0.setX(10.5);
    p0.setY(-4.2);

    CHECK(p0.getX() == doctest::Approx(10.5));
    CHECK(p0.getY() == doctest::Approx(-4.2));
  }

  SUBCASE("testing distance method") {
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
    const double constant0{0.};
    const double constant1{2.};
    const double constant2{-3.};
    point::Point q00 = constant0 * p0;
    point::Point q01 = constant1 * p0;
    point::Point q02 = constant2 * p0;
    point::Point q20 = constant0 * p2;
    point::Point q21 = constant1 * p2;
    point::Point q22 = constant2 * p2;
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
    const double constant1{2.};
    const double constant2{-3.};
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

/////////////////TESTING BOID CLASS//////////////////////////////

const point::Point pos1(3., -2.);
const point::Point vel1(5., -1.);
const point::Point pos2 =
    pos1 + 0.8 * d_ * 0.5 *
               point::Point(vel1.getX() - std::sqrt(3) * vel1.getY(),
                            std::sqrt(3) * vel1.getX() + vel1.getY()) /
               vel1.distance();
const point::Point vel2(-8.5, -6.);
const point::Point pos3 =
    pos1 + 0.8 * d_ * 0.5 *
               point::Point(vel1.getX() + std::sqrt(3) * vel1.getY(),
                            -std::sqrt(3) * vel1.getX() + vel1.getY()) /
               vel1.distance();
const point::Point vel3(-7.3, 2.5);
const point::Point pos4 =
    pos1 + 0.5 * prey_ds_ * 0.5 *
               point::Point(vel1.getX() - std::sqrt(3) * vel1.getY(),
                            std::sqrt(3) * vel1.getX() + vel1.getY()) /
               vel1.distance();
const point::Point vel4(-1., -2.);
const point::Point pos5 =
    pos1 + 0.5 * prey_ds_ * 0.5 *
               point::Point(vel1.getX() + std::sqrt(3) * vel1.getY(),
                            -std::sqrt(3) * vel1.getX() + vel1.getY()) /
               vel1.distance();
const point::Point vel5(-5., -3.2);

TEST_CASE("Testing Prey class") {
  const double s{0.1};
  const double a{0.1};
  const double c{0.004};
  const double r{s * 6};
  boid::Prey b0;
  boid::Prey b1(pos1, vel1);
  boid::Prey b2(pos2, vel2);
  boid::Prey b3(pos3, vel3);
  boid::Prey b4(pos4, vel4);
  boid::Prey b5;
  b5.setBoid(pos5, vel5);

  point::Point v0 = b0.getVelocity();
  point::Point v1 = vel1;
  point::Point v2 = vel2;
  point::Point v3 = vel3;
  point::Point v4 = vel4;
  point::Point v5 = vel5;

  std::vector<std::shared_ptr<boid::Boid>> near_b1{
      std::make_shared<boid::Prey>(b2), std::make_shared<boid::Prey>(b3),
      std::make_shared<boid::Prey>(b4), std::make_shared<boid::Prey>(b5)};
  std::vector<std::shared_ptr<boid::Boid>> near_b1_void;

  SUBCASE("Testing getters") {
    CHECK(b0.getPosition() == point::Point(0., 0.));
    CHECK(b0.getVelocity() == point::Point(0., 0.));
    CHECK(b1.getPosition() == point::Point(3., -2.));
    CHECK(b1.getVelocity() == point::Point(5., -1.));
  }

  SUBCASE("Testing setters") {
    CHECK(b5.getPosition().getX() == doctest::Approx(pos5.getX()));
    CHECK(b5.getPosition().getY() == doctest::Approx(pos5.getY()));
    CHECK(b5.getVelocity() == point::Point(-5., -3.2));
  }
  SUBCASE("Testing angle method") {
    CHECK(b0.angle(b1) == doctest::Approx(0.00000000));
    CHECK(b1.angle(b1) == doctest::Approx(0.00000000));
    CHECK(b1.angle(b2) == doctest::Approx(1.04719755));
    CHECK(b1.angle(b3) == doctest::Approx(-1.04719755));
  }

  SUBCASE("Testing separation method") {
    const double sep1_x = -s * 0.5 * prey_ds_ * vel1.getX() / vel1.distance();
    const double sep1_y = -s * 0.5 * prey_ds_ * vel1.getY() / vel1.distance();
    CHECK(b1.separation(s, prey_ds_, near_b1_void).getX() ==
          doctest::Approx(0.00000000));
    CHECK(b1.separation(s, prey_ds_, near_b1_void).getY() ==
          doctest::Approx(0.00000000));
    CHECK(b1.separation(s, prey_ds_, near_b1).getX() ==
          doctest::Approx(sep1_x));
    CHECK(b1.separation(s, prey_ds_, near_b1).getY() ==
          doctest::Approx(sep1_y));
  }

  SUBCASE("Testing alignment method") {
    CHECK(b1.alignment(a, near_b1_void).getX() == doctest::Approx(0.00000000));
    CHECK(b1.alignment(a, near_b1_void).getY() == doctest::Approx(0.00000000));
    const double al1_x =
        a * ((vel2.getX() + vel3.getX() + vel4.getX() + vel5.getX()) / 4. - 5.);
    const double al1_y =
        a * ((vel2.getY() + vel3.getY() + vel4.getY() + vel5.getY()) / 4. + 1.);

    CHECK(b1.alignment(a, near_b1).getX() == doctest::Approx(al1_x));
    CHECK(b1.alignment(a, near_b1).getY() == doctest::Approx(al1_y));
  }

  SUBCASE("Testing cohesion method") {
    CHECK(b1.cohesion(c, near_b1_void).getX() == doctest::Approx(0.00000000));
    CHECK(b1.cohesion(c, near_b1_void).getX() == doctest::Approx(0.00000000));

    const double coh1_x =
        c * (0.5 * prey_ds_ + 0.8 * d_) / 4 * vel1.getX() / vel1.distance();
    const double coh1_y =
        c * (0.5 * prey_ds_ + 0.8 * d_) / 4 * vel1.getY() / vel1.distance();
    CHECK(b1.cohesion(c, near_b1).getX() == doctest::Approx(coh1_x));
    CHECK(b1.cohesion(c, near_b1).getX() == doctest::Approx(coh1_x));
    CHECK(b1.cohesion(c, near_b1).getX() == doctest::Approx(coh1_x));
    CHECK(b1.cohesion(c, near_b1).getY() == doctest::Approx(coh1_y));
  }

  SUBCASE("Testing repulsion method") {
    // here we apply boid::Prey::repulsion on near_b1, which identifies the
    // prey near b1, even if it was initially intended to be applied on
    // near_predators.
    CHECK(b1.repulsion(r, near_b1_void).getX() == doctest::Approx(0.00000000));
    CHECK(b1.repulsion(r, near_b1_void).getY() == doctest::Approx(0.00000000));
    const double rep1_x =
        -r * (0.5 * prey_ds_ + 0.8 * d_) * vel1.getX() / vel1.distance();
    const double rep1_y =
        -r * (0.5 * prey_ds_ + 0.8 * d_) * vel1.getY() / vel1.distance();
    CHECK(b1.repulsion(r, near_b1).getX() == doctest::Approx(rep1_x));
    CHECK(b1.repulsion(r, near_b1).getY() == doctest::Approx(rep1_y));
  }

  SUBCASE("Testing clamp method") {
    b0.clamp(prey_min_speed, prey_max_speed, v0);
    b1.clamp(prey_min_speed, prey_max_speed, v1);
    b2.clamp(prey_min_speed, prey_max_speed, v2);
    b3.clamp(prey_min_speed, prey_max_speed, v3);
    b4.clamp(prey_min_speed, prey_max_speed, v4);
    b5.clamp(prey_min_speed, prey_max_speed, v5);

    CHECK(v0.getX() == doctest::Approx(prey_min_speed));
    CHECK(v0.getY() == doctest::Approx(0.00000000));
    CHECK(v1.getX() ==
          doctest::Approx(prey_max_speed * vel1.getX() / vel1.distance()));
    CHECK(v1.getY() ==
          doctest::Approx(prey_max_speed * vel1.getY() / vel1.distance()));
    CHECK(v2.getX() ==
          doctest::Approx(prey_max_speed * vel2.getX() / vel2.distance()));
    CHECK(v2.getY() ==
          doctest::Approx(prey_max_speed * vel2.getY() / vel2.distance()));
    CHECK(v3.getX() ==
          doctest::Approx(prey_max_speed * vel3.getX() / vel3.distance()));
    CHECK(v3.getY() ==
          doctest::Approx(prey_max_speed * vel3.getY() / vel3.distance()));
    CHECK(v4.getX() ==
          doctest::Approx(prey_min_speed * vel4.getX() / vel4.distance()));
    CHECK(v4.getY() ==
          doctest::Approx(prey_min_speed * vel4.getY() / vel4.distance()));
    CHECK(v5.getX() ==
          doctest::Approx(prey_max_speed * vel5.getX() / vel5.distance()));
    CHECK(v5.getY() ==
          doctest::Approx(prey_max_speed * vel5.getY() / vel5.distance()));
  }
}

TEST_CASE("Testing Predator class") {
  double ch{0.004};
  boid::Predator p0;
  boid::Predator p1(pos1, vel1);
  boid::Predator p2(pos2, vel2);
  boid::Predator p3(pos3, vel3);
  boid::Predator p4;
  const point::Point pos6 =
      pos1 + 0.5 * d_ * 0.5 *
                 point::Point(vel1.getX() - std::sqrt(3) * vel1.getY(),
                              std::sqrt(3) * vel1.getX() + vel1.getY()) /
                 vel1.distance();
  p4.setBoid(pos6, vel4);
  boid::Predator p5;
  const point::Point pos7 =
      pos1 + 0.5 * d_ * 0.5 *
                 point::Point(vel1.getX() + std::sqrt(3) * vel1.getY(),
                              -std::sqrt(3) * vel1.getX() + vel1.getY()) /
                 vel1.distance();
  p5.setBoid(pos7, vel5);

  point::Point v0 = p0.getVelocity();
  point::Point v1 = vel1;
  point::Point v2 = vel2;
  point::Point v3 = vel3;
  point::Point v4 = vel4;
  point::Point v5 = vel5;

  std::vector<std::shared_ptr<boid::Boid>> near_p1{
      std::make_shared<boid::Predator>(p2),
      std::make_shared<boid::Predator>(p3),
      std::make_shared<boid::Predator>(p4),
      std::make_shared<boid::Predator>(p5)};
  std::vector<std::shared_ptr<boid::Boid>> near_p1_void;

  SUBCASE("Testing getters") {
    CHECK(p0.getPosition() == point::Point(0., 0.));
    CHECK(p0.getVelocity() == point::Point(0., 0.));
    CHECK(p1.getPosition() == point::Point(3., -2.));
    CHECK(p1.getVelocity() == point::Point(5., -1.));
  }

  SUBCASE("Testing setters") {
    CHECK(p4.getPosition().getX() == doctest::Approx(pos6.getX()));
    CHECK(p4.getPosition().getY() == doctest::Approx(pos6.getY()));
    CHECK(p4.getVelocity() == point::Point(-1., -2.));
    CHECK(p5.getPosition().getX() == doctest::Approx(pos7.getX()));
    CHECK(p5.getPosition().getY() == doctest::Approx(pos7.getY()));
    CHECK(p5.getVelocity() == point::Point(-5., -3.2));
  }

  SUBCASE("Testing chase method") {
    // here we apply boidd::Predator::chase on near_p1, which identifies the
    // predators near p1, even if it was initially intended to be applied on
    // near_boids.
    CHECK(p1.chase(ch, near_p1_void).getX() == doctest::Approx(0.00000000));
    CHECK(p1.chase(ch, near_p1_void).getY() == doctest::Approx(0.00000000));

    double chase1_x = ch * d_ * 1.3 * vel1.getX() / vel1.distance();
    double chase1_y = ch * d_ * 1.3 * vel1.getY() / vel1.distance();

    CHECK(p1.chase(ch, near_p1).getX() == doctest::Approx(chase1_x));
    CHECK(p1.chase(ch, near_p1).getY() == doctest::Approx(chase1_y));
  }
  SUBCASE("Testing clamp method") {
    p0.clamp(predator_min_speed, predator_max_speed, v0);
    p1.clamp(predator_min_speed, predator_max_speed, v1);
    p2.clamp(predator_min_speed, predator_max_speed, v2);
    p3.clamp(predator_min_speed, predator_max_speed, v3);
    p4.clamp(predator_min_speed, predator_max_speed, v4);
    p5.clamp(predator_min_speed, predator_max_speed, v5);

    CHECK(v0.getX() == doctest::Approx(predator_min_speed));
    CHECK(v0.getY() == doctest::Approx(0.00000000));
    CHECK(v1.getX() ==
          doctest::Approx(predator_max_speed * vel1.getX() / vel1.distance()));
    CHECK(v1.getY() ==
          doctest::Approx(predator_max_speed * vel1.getY() / vel1.distance()));
    CHECK(v2.getX() ==
          doctest::Approx(predator_max_speed * vel2.getX() / vel2.distance()));
    CHECK(v2.getY() ==
          doctest::Approx(predator_max_speed * vel2.getY() / vel2.distance()));
    CHECK(v3.getX() ==
          doctest::Approx(predator_max_speed * vel3.getX() / vel3.distance()));
    CHECK(v3.getY() ==
          doctest::Approx(predator_max_speed * vel3.getY() / vel3.distance()));
    CHECK(v4.getX() ==
          doctest::Approx(predator_min_speed * vel4.getX() / vel4.distance()));
    CHECK(v4.getY() ==
          doctest::Approx(predator_min_speed * vel4.getY() / vel4.distance()));
    CHECK(v5.getX() ==
          doctest::Approx(predator_max_speed * vel5.getX() / vel5.distance()));
    CHECK(v5.getY() ==
          doctest::Approx(predator_max_speed * vel5.getY() / vel5.distance()));
  }
}

/////////////// TESTING FLOCK CLASS /////////////////

TEST_CASE("Testing Flock class") {
  auto prey1 =
      std::make_shared<boid::Prey>(point::Point(1, 1), point::Point(1, 0));
  auto prey2 =
      std::make_shared<boid::Prey>(point::Point(2, 2), point::Point(0, 1));
  auto prey3 =
      std::make_shared<boid::Prey>(point::Point(3, 3), point::Point(-1, 0));
  auto prey4 =
      std::make_shared<boid::Prey>(point::Point(4, 4), point::Point(0, -1));

  auto predator1 =
      std::make_shared<boid::Predator>(point::Point(5, 5), point::Point(1, 1));
  auto predator2 = std::make_shared<boid::Predator>(point::Point(6, 6),
                                                    point::Point(-1, -1));

  std::vector<std::shared_ptr<boid::Prey>> prey_flock = {prey1, prey2, prey3,
                                                         prey4};
  std::vector<std::shared_ptr<boid::Predator>> predator_flock = {predator1,
                                                                 predator2};

  flock::SpeedLimits custom_speed_limits{5.0, 10.0, 3.0, 6.0};

  flock::Flock f0(5, 2);
  flock::Flock f1(prey_flock, predator_flock, custom_speed_limits);

  SUBCASE("Testing getPreyNum, getPredatorNum, getFlockSize methods") {
    CHECK(f0.getPreyNum() == 5);
    CHECK(f0.getPredatorsNum() == 2);
    CHECK(f0.getFlockSize() == 7);
    CHECK(f1.getPreyNum() == 4);
    CHECK(f1.getPredatorsNum() == 2);
    CHECK(f1.getFlockSize() == 6);
  }

  SUBCASE(
      "Testing first constructor, getFlightParameters, getSpeedLimits "
      "methods") {
    CHECK(f0.getPreyFlock().size() == 0);
    CHECK(f0.getPredatorFlock().size() == 0);

    flock::FlightParameters flight_parameters_f0 = f0.getFlightParameters();

    CHECK(flight_parameters_f0.separation == doctest::Approx(0.1));
    CHECK(flight_parameters_f0.alignment == doctest::Approx(0.1));
    CHECK(flight_parameters_f0.cohesion == doctest::Approx(0.004));
    CHECK(flight_parameters_f0.repulsion == doctest::Approx(0.6));
    CHECK(flight_parameters_f0.chase == doctest::Approx(0.008));

    flock::SpeedLimits speed_limits_f0 = f0.getSpeedLimits();

    CHECK(speed_limits_f0.prey_min == doctest::Approx(7.));
    CHECK(speed_limits_f0.prey_max == doctest::Approx(12.));
    CHECK(speed_limits_f0.predator_min == doctest::Approx(5.));
    CHECK(speed_limits_f0.predator_max == doctest::Approx(8.));
  }

  SUBCASE(
      "Testing second constructor, getFlightParameters, getSpeedLimits "
      "methods") {
    CHECK(f1.getPreyFlock().size() == 4);
    CHECK(f1.getPredatorFlock().size() == 2);
    flock::FlightParameters flight_parameters_f1 = f1.getFlightParameters();

    CHECK(flight_parameters_f1.separation == doctest::Approx(0.1));
    CHECK(flight_parameters_f1.alignment == doctest::Approx(0.1));
    CHECK(flight_parameters_f1.cohesion == doctest::Approx(0.004));
    CHECK(flight_parameters_f1.repulsion == doctest::Approx(0.6));
    CHECK(flight_parameters_f1.chase == doctest::Approx(0.008));

    flock::SpeedLimits speed_limits_f1 = f1.getSpeedLimits();

    CHECK(speed_limits_f1.prey_min == doctest::Approx(5.));
    CHECK(speed_limits_f1.prey_max == doctest::Approx(10.));
    CHECK(speed_limits_f1.predator_min == doctest::Approx(3.));
    CHECK(speed_limits_f1.predator_max == doctest::Approx(6.));
  }

  SUBCASE("Testing getPreyFlock, getPredatorFlock methods") {
    const auto f1_prey = f1.getPreyFlock();
    const auto f1_predator = f1.getPredatorFlock();

    CHECK(f1_prey[2]->getPosition().getX() == doctest::Approx(3.));
    CHECK(f1_prey[2]->getPosition().getY() == doctest::Approx(3.));
    CHECK(f1_prey[2]->getVelocity().getX() == doctest::Approx(-1.));
    CHECK(f1_prey[2]->getVelocity().getY() == doctest::Approx(0.));
    CHECK(f1_predator[1]->getPosition().getX() == doctest::Approx(6.));
    CHECK(f1_predator[1]->getPosition().getY() == doctest::Approx(6.));
    CHECK(f1_predator[1]->getVelocity().getX() == doctest::Approx(-1.));
    CHECK(f1_predator[1]->getVelocity().getY() == doctest::Approx(-1.));
  }

  SUBCASE("Testing getDistanceParameters method") {
    const std::array<double, 3> distance_parameters_f0 =
        flock::Flock::getDistanceParameters();

    CHECK(distance_parameters_f0[0] == doctest::Approx(75.0));
    CHECK(distance_parameters_f0[1] == doctest::Approx(20.0));
    CHECK(distance_parameters_f0[2] == doctest::Approx(37.5));
  }

  SUBCASE("Testing nearPrey, nearPredators methods") {
    auto prey5 = std::make_shared<boid::Prey>(point::Point(2, 100),
                                              point::Point(0., -1.));

    auto predator3 = std::make_shared<boid::Predator>(point::Point(2., 0.),
                                                      point::Point(0., -1.));

    auto predator4 = std::make_shared<boid::Predator>(point::Point(2, 120),
                                                      point::Point(1, 1));

    std::vector<std::shared_ptr<boid::Prey>> prey_flock_2 = {
        prey1, prey2, prey3, prey4, prey5};
    std::vector<std::shared_ptr<boid::Predator>> predator_flock_2 = {
        predator1, predator2, predator3, predator4};

    flock::Flock f2(prey_flock_2, predator_flock_2, custom_speed_limits);

    const auto prey2_near_prey = f2.nearPrey(1, true);
    CHECK(prey2_near_prey.size() == 2);

    const auto prey2_near_predators = f2.nearPredators(1, true);
    CHECK(prey2_near_predators.size() == 2);

    const auto prey5_near_prey = f2.nearPrey(4, true);
    CHECK(prey5_near_prey.size() == 0);

    const auto prey5_near_predators = f2.nearPredators(4, true);
    CHECK(prey5_near_predators.size() == 0);

    const auto pred2_near_prey = f2.nearPrey(1, false);
    CHECK(pred2_near_prey.size() == 4);

    const auto pred2_near_predators = f2.nearPredators(1, false);
    CHECK(pred2_near_predators.size() == 2);

    const auto pred3_near_prey = f2.nearPrey(2, false);
    CHECK(prey5_near_prey.size() == 0);

    const auto pred3_near_predators = f2.nearPredators(2, false);
    CHECK(prey5_near_predators.size() == 0);

    CHECK(prey2_near_prey[0]->getPosition().getX() == doctest::Approx(3.));
    CHECK(prey2_near_prey[0]->getPosition().getY() == doctest::Approx(3.));
    CHECK(prey2_near_prey[1]->getPosition().getX() == doctest::Approx(4.));
    CHECK(prey2_near_prey[1]->getPosition().getY() == doctest::Approx(4.));

    CHECK(prey2_near_predators[0]->getPosition().getX() == doctest::Approx(5.));
    CHECK(prey2_near_predators[0]->getPosition().getY() == doctest::Approx(5.));
    CHECK(prey2_near_predators[1]->getPosition().getX() == doctest::Approx(6.));
    CHECK(prey2_near_predators[1]->getPosition().getY() == doctest::Approx(6.));

    CHECK(pred2_near_prey[0]->getPosition().getX() == doctest::Approx(1.));
    CHECK(pred2_near_prey[0]->getPosition().getY() == doctest::Approx(1.));
    CHECK(pred2_near_prey[1]->getPosition().getX() == doctest::Approx(2.));
    CHECK(pred2_near_prey[1]->getPosition().getY() == doctest::Approx(2.));
    CHECK(pred2_near_prey[2]->getPosition().getX() == doctest::Approx(3.));
    CHECK(pred2_near_prey[2]->getPosition().getY() == doctest::Approx(3.));
    CHECK(pred2_near_prey[3]->getPosition().getX() == doctest::Approx(4.));
    CHECK(pred2_near_prey[3]->getPosition().getY() == doctest::Approx(4.));

    CHECK(pred2_near_predators[0]->getPosition().getX() == doctest::Approx(5.));
    CHECK(pred2_near_predators[0]->getPosition().getY() == doctest::Approx(5.));
    CHECK(pred2_near_predators[1]->getPosition().getX() == doctest::Approx(2.));
    CHECK(pred2_near_predators[1]->getPosition().getY() == doctest::Approx(0.));
  }

  SUBCASE("Testing generateBoids method") {
    flock::Flock f3(4, 2);
    f3.generateBoids();

    CHECK(f3.getPreyFlock().size() == 4);
    CHECK(f3.getPredatorFlock().size() == 2);

    for (const auto& p : f3.getPreyFlock()) {
      CHECK(p != nullptr);
    }
    for (const auto& p : f3.getPredatorFlock()) {
      CHECK(p != nullptr);
    }

    for (const auto& p : f3.getPreyFlock()) {
      CHECK(p->getPosition().getX() >= 0.0);
      CHECK(p->getPosition().getX() <= graphics::window_width);
      CHECK(p->getPosition().getY() >= 0.0);
      CHECK(p->getPosition().getY() <= graphics::window_height);
    }

    for (const auto& p : f3.getPreyFlock()) {
      CHECK(p->getVelocity().distance() >= 2.0);
      CHECK(p->getVelocity().distance() <= 5.0);
    }

    auto first_positions = f3.getPreyFlock();
    f3.generateBoids();
    auto second_positions = f3.getPreyFlock();

    bool any_changed = false;
    for (size_t i = 0; i < first_positions.size(); ++i) {
      if (first_positions[i]->getPosition().getX() !=
          second_positions[i]->getPosition().getX()) {
        any_changed = true;
        break;
      }
    }
    CHECK(any_changed);
  }
  SUBCASE("Testing updateBoid method on the second prey of f1") {
    const double s{0.1};
    const double a{0.1};
    const double c{0.004};
    const double r{s * 6};

    const auto near_prey = f1.nearPrey(1, true);
    const auto near_predators = f1.nearPredators(1, true);

    CHECK(near_prey.size() == 2);
    CHECK(near_predators.size() == 2);

    point::Point vel = prey_flock[1]->getVelocity();
    vel += prey_flock[1]->repulsion(r, near_predators) +
           prey_flock[1]->separation(s, prey_ds_, near_prey) +
           prey_flock[1]->alignment(a, near_prey) +
           prey_flock[1]->cohesion(c, near_prey);
    prey_flock[1]->clamp(5, 10, vel);
    CHECK(f1.updateBoid(1, true, 1)[1].getX() == doctest::Approx(vel.getX()));
    CHECK(f1.updateBoid(1, true, 1)[1].getY() == doctest::Approx(vel.getY()));
    point::Point pos = point::Point(2., 2.) + vel + point::Point(1400., 800);
    CHECK(f1.updateBoid(1, true, 1)[0].getX() == doctest::Approx(pos.getX()));
    CHECK(f1.updateBoid(1, true, 1)[0].getY() == doctest::Approx(pos.getY()));
  }
  SUBCASE(" Testing updateBoid on the second predator of f1") {
    const double s{0.1};
    const double ch{0.008};

    const auto near_prey = f1.nearPrey(1, false);
    const auto near_predators = f1.nearPredators(1, false);

    CHECK(near_prey.size() == 4);
    CHECK(near_predators.size() == 1);

    point::Point vel = predator_flock[1]->getVelocity();
    vel += predator_flock[1]->separation(s, predator_ds_, near_predators) +
           predator_flock[1]->chase(ch, near_prey);
    predator_flock[1]->clamp(3, 6, vel);

    CHECK(f1.updateBoid(1, false, 1)[1].getX() == doctest::Approx(vel.getX()));
    CHECK(f1.updateBoid(1, false, 1)[1].getY() == doctest::Approx(vel.getY()));
    point::Point pos = point::Point(6., 6.) + vel;
    CHECK(f1.updateBoid(1, false, 1)[0].getX() == doctest::Approx(pos.getX()));
    CHECK(f1.updateBoid(1, false, 1)[0].getY() == doctest::Approx(pos.getY()));
  }
  SUBCASE("Testing updateFlock") {
    double dt = 1.0;

    const auto preys = prey_flock;
    const auto preds = predator_flock;

    const std::size_t n_prey = preys.size();
    const std::size_t n_pred = preds.size();

    std::vector<point::Point> expected_prey_pos;
    std::vector<point::Point> expected_prey_vel;
    std::vector<point::Point> expected_pred_pos;
    std::vector<point::Point> expected_pred_vel;

    expected_prey_pos.reserve(n_prey);
    expected_prey_vel.reserve(n_prey);
    expected_pred_pos.reserve(n_pred);
    expected_pred_vel.reserve(n_pred);

    for (std::size_t i = 0; i < n_prey; ++i) {
      std::array<point::Point, 2> res = f1.updateBoid(i, true, dt);
      expected_prey_pos.push_back(res[0]);
      expected_prey_vel.push_back(res[1]);
    }

    for (std::size_t i = 0; i < n_pred; ++i) {
      std::array<point::Point, 2> res = f1.updateBoid(i, false, dt);
      expected_pred_pos.push_back(res[0]);
      expected_pred_vel.push_back(res[1]);
    }

    f1.updateFlock(dt);

    for (std::size_t i = 0; i < n_prey; ++i) {
      CHECK(preys[i]->getPosition().getX() ==
            doctest::Approx(expected_prey_pos[i].getX()));
      CHECK(preys[i]->getPosition().getY() ==
            doctest::Approx(expected_prey_pos[i].getY()));
      CHECK(preys[i]->getVelocity().getX() ==
            doctest::Approx(expected_prey_vel[i].getX()));
      CHECK(preys[i]->getVelocity().getY() ==
            doctest::Approx(expected_prey_vel[i].getY()));
    }

    for (std::size_t i = 0; i < n_pred; ++i) {
      CHECK(preds[i]->getPosition().getX() ==
            doctest::Approx(expected_pred_pos[i].getX()));
      CHECK(preds[i]->getPosition().getY() ==
            doctest::Approx(expected_pred_pos[i].getY()));
      CHECK(preds[i]->getVelocity().getX() ==
            doctest::Approx(expected_pred_vel[i].getX()));
      CHECK(preds[i]->getVelocity().getY() ==
            doctest::Approx(expected_pred_vel[i].getY()));
    }
  }
}

/////////////// TESTING STATISTICS STRUCT /////////

TEST_CASE("Testing Statistics struct") {
  statistics::Statistics stats0;
  statistics::Statistics stats1(15., 11., 12., 5.);

  CHECK(stats0.mean_distance == 0.);
  CHECK(stats0.dev_distance == 0.);
  CHECK(stats0.mean_velocity == 0.);
  CHECK(stats0.dev_velocity == 0.);

  CHECK(stats1.mean_distance == 15.);
  CHECK(stats1.dev_distance == 11.);
  CHECK(stats1.mean_velocity == 12.);
  CHECK(stats1.dev_velocity == 5.);
}

///////////// TESTING GRAPHICS ///////////////////

TEST_CASE("Graphics and Main functionality") {
  SUBCASE("angleDegFromVelocity") {
    CHECK(std::fabs(graphics::angleDegFromVelocity(1, 0) - 0.f) < 1e-5);
    CHECK(std::fabs(graphics::angleDegFromVelocity(0, 1) - 90.f) < 1e-5);
    CHECK(std::fabs(graphics::angleDegFromVelocity(-1, 0) - 180.f) < 1e-5);
    CHECK(std::fabs(graphics::angleDegFromVelocity(0, -1) + 90.f) < 1e-5);
  }
  SUBCASE("makeBoidTriangle") {
    auto triangle =
        graphics::makeBoidTriangle(10.f, 1.f, sf::Color::Red, sf::Color::Blue);
    CHECK(triangle.getPointCount() == 3);
    CHECK(triangle.getFillColor() == sf::Color::Red);
    CHECK(triangle.getOutlineColor() == sf::Color::Blue);
    CHECK(triangle.getOutlineThickness() == 1.f);
  }
  SUBCASE("drawBoid on window") {
    auto window = graphics::makeWindow(200, 200, "Test Window");
    REQUIRE(window);
    window->clear(sf::Color::Black);
    CHECK_NOTHROW(
        graphics::drawBoid(*window, 50, 50, 1, 0, graphics::Style(), true));
    CHECK_NOTHROW(
        graphics::drawBoid(*window, 100, 100, -1, 1, graphics::Style(), false));
    window->display();
  }
  SUBCASE("drawFrame with empty flock") {
    flock::Flock flock(0, 0);
    auto window = graphics::makeWindow(200, 200, "Frame Test");
    REQUIRE(window);
    CHECK_NOTHROW(graphics::drawFrame(*window, flock, graphics::Style()));
    window->display();
  }
  SUBCASE("drawFrame with some boids") {
    flock::Flock flock(2, 1);
    flock.generateBoids();
    auto window = graphics::makeWindow(400, 400, "Frame Test 2");
    REQUIRE(window);
    CHECK_NOTHROW(graphics::drawFrame(*window, flock, graphics::Style()));
    window->display();
  }
  SUBCASE("loadBackground") {
    // Assuming "background.png" does not exist: should return false
    CHECK(graphics::loadBackground("non_existing_file.png") == false);
    // You can test true if a valid image exists in project
  }
  SUBCASE("makeWindow returns valid unique_ptr") {
    auto window = graphics::makeWindow(300, 300, "Window Test");
    REQUIRE(window);
    CHECK(window->isOpen() == true);
  }
  SUBCASE("SFML Event polling") {
    auto window = graphics::makeWindow(200, 200, "Event Test");
    REQUIRE(window);
    sf::Event event;
    CHECK_NOTHROW(window->pollEvent(event));
  }
}
