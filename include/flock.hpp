#ifndef FLOCK_HPP
#define FLOCK_HPP

#include <SFML/Graphics/VertexArray.hpp>
#include <array>
#include <cmath>
#include <memory>
#include <vector>

#include "boid.hpp"
#include "statistics.hpp"

namespace flock {

struct FlightParameters {
  double separation;
  double alignment;
  double cohesion;
  double repulsion;
  double chase;
};

struct SpeedLimits {
  double prey_min;
  double prey_max;
  double predator_min;
  double predator_max;
};

class Flock {
 private:
  std::mt19937 mt_{std::random_device{}()};
  std::size_t n_prey_;
  std::size_t n_predators_;

  std::vector<std::shared_ptr<boid::Prey>> prey_flock_;
  std::vector<std::shared_ptr<boid::Predator>> predator_flock_;

  static constexpr double prey_sight_angle_ = 2. / 3 * M_PI;
  static constexpr double predator_sight_angle_ = 0.5 * M_PI;

  FlightParameters flight_params_;
  SpeedLimits speed_limits_;

  static constexpr double boids_dis_ = 75.;  // radius for near boids
  static constexpr double prey_sep_ = 20.;   // separation radius for prey
  static constexpr double predator_sep_ =
      boids_dis_ * 0.5;  // separation radius for predators

 public:
  Flock(std::size_t nPrey, std::size_t nPredators);

  Flock(const std::vector<std::shared_ptr<boid::Prey>>& prey,
        const std::vector<std::shared_ptr<boid::Predator>>& predators,
        double preyMaxSpeed, double predatorMaxSpeed, double preyMinSpeed,
        double predatorMinSpeed);

  std::size_t getPreyNum() const;
  std::size_t getPredatorsNum() const;
  std::size_t getFlockSize() const;

  std::vector<std::shared_ptr<boid::Prey>> getPreyFlock() const;
  std::vector<std::shared_ptr<boid::Predator>> getPredatorFlock() const;

  FlightParameters getFlightParameters() const;
  // SpeedLimits getSpeedLimits() const;

  static std::array<double, 3> getDistancesParameters();

  void setFlightParameters(std::istream& in, std::ostream& out);

  void generateBoids();  // generates boids with random positions and velocities

  std::vector<std::shared_ptr<boid::Boid>> nearPrey(const std::size_t i,
                                                    const bool is_prey) const;
  // return a vector containing pointers to prey near the current, i-th boid.

  std::vector<std::shared_ptr<boid::Boid>> nearPredators(
      const std::size_t i, const bool is_prey) const;

  // return a vector containing pointers to predators near the current, i-th
  // boid.

  std::array<point::Point, 2> updateBoid(sf::VertexArray& triangles, size_t i,
                                         bool is_prey)
      const;  // evaluates the new position and velocity of a bird in the flock.

  void updateFlock(sf::VertexArray& triangles) const;

  // evaluates the relevant statistical quantities
  statistics::Statistics statistics() const;
};

}  // namespace flock
#endif