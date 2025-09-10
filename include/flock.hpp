#ifndef FLOCK_HPP
#define FLOCK_HPP

#include <array>
#include <iostream>
#include <memory>
#include <random>
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

  FlightParameters flight_parameters_;
  SpeedLimits speed_limits_;

  static constexpr double d_ = 75.;        // radius for near boids
  static constexpr double prey_ds_ = 20.;  // separation radius for prey
  static constexpr double predator_ds_ = d_ * 0.5;  // and for predators

 public:
  Flock(std::size_t n_prey, std::size_t n_predators);

  Flock(const std::vector<std::shared_ptr<boid::Prey>>& prey,
        const std::vector<std::shared_ptr<boid::Predator>>& predators,
        const SpeedLimits& speed_limits);

  std::size_t getPreyNum() const;
  std::size_t getPredatorsNum() const;
  std::size_t getFlockSize() const;

  std::vector<std::shared_ptr<boid::Prey>> getPreyFlock() const;
  std::vector<std::shared_ptr<boid::Predator>> getPredatorFlock() const;

  FlightParameters getFlightParameters() const;
  // SpeedLimits getSpeedLimits() const; non serve

  static std::array<double, 3> getDistancesParameters();

  void setFlockSize();

  void setFlightParameters();

  void generateBoids();

  std::vector<std::shared_ptr<boid::Boid>> nearPrey(const std::size_t i,
                                                    const bool is_prey) const;

  std::vector<std::shared_ptr<boid::Boid>> nearPredators(
      const std::size_t i, const bool is_prey) const;

  std::array<point::Point, 2> updateBoid(std::size_t i, bool is_prey,
                                         double dt) const;

  void updateFlock(double dt) const;

  statistics::Statistics statistics() const;
};

}  // namespace flock
#endif
