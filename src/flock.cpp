#include "../include/flock.hpp"

#include <SFML/Graphics/VertexArray.hpp>
#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

#include "../include/boid.hpp"
#include "../include/graphic.hpp"
#include "../include/point.hpp"
#include "../include/statistics.hpp"
#include "../include/triangle.hpp"

namespace flock {

// Costruttore principale con valori di default per parametri
Flock::Flock(const std::size_t nPrey, const std::size_t nPredators)
    : n_prey_(nPrey),
      n_predators_(nPredators),
      flight_parameters_{0.1, 0.1, 0.004, 0.6, 0.008},
      speed_limits_{7., 12., 5., 8.} {
  prey_flock_.reserve(n_prey_);
  predator_flock_.reserve(n_predators_);
}

// Costruttore con vettori esistenti e parametri personalizzati
Flock::Flock(const std::vector<std::shared_ptr<boid::Prey>>& prey,
             const std::vector<std::shared_ptr<boid::Predator>>& predators,
             const double preyMaxSpeed, const double predatorMaxSpeed,
             const double preyMinSpeed, const double predatorMinSpeed)
    : n_prey_(prey.size()),
      n_predators_(predators.size()),
      prey_flock_(prey),
      predator_flock_(predators),
      flight_params_{0.1, 0.1, 0.004, 0.6, 0.008},
      speed_limits_{preyMinSpeed, preyMaxSpeed, predatorMinSpeed,
                    predatorMaxSpeed} {}

std::size_t Flock::getPreyNum() const { return n_prey_; }
std::size_t Flock::getPredatorsNum() const { return n_predators_; }
std::size_t Flock::getFlockSize() const { return n_prey_ + n_predators_; }

std::vector<std::shared_ptr<boid::Prey>> Flock::getPreyFlock() const {
  return prey_flock_;
}

std::vector<std::shared_ptr<boid::Predator>> Flock::getPredatorFlock() const {
  return predator_flock_;
}

FlightParameters Flock::getFlightParameters() const { return flight_params_; }

// SpeedLimits Flock::getSpeedLimits() const { return speed_limits_; }

std::array<double, 3> Flock::getDistancesParameters() {
  return {boids_dis_, prey_sep_, predator_sep_};
}
void Flock::setFlightParameters(std::istream& in, std::ostream& out) {
  char statement;
  out << "\nWould you like to customize the parameters of the simulation? "
         "(Y/n) ";
  in >> statement;

  if (in.fail()) {
    throw std::runtime_error("Input failed.");
  }

  if (statement == 'Y' || statement == 'y') {
    const double s = graphic_par::getPositiveDouble(
        "\nEnter the separation coefficient: ", in, out);
    const double a = graphic_par::getPositiveDouble(
        "Enter the alignment coefficient: ", in, out);
    const double c = graphic_par::getPositiveDouble(
        "Enter the cohesion coefficient: ", in, out);

    flight_parameters_.separation = s;
    flight_parameters_.alignment = a;
    flight_parameters_.cohesion = c;

    flight_parameters_.repulsion = s * 6;
    flight_parameters_.chase = c * 2;

  } else if (statement == 'N' || statement == 'n') {
    out << "\nThe simulation parameters are set as default "
           "(separation= 0.1, alignment = 0.1, cohesion = 0.004)\n";

  } else {
    throw std::domain_error(
        "Error: Invalid input. The program will now terminate.");
  }
}

void Flock::generateBoids() {
  std::uniform_real_distribution<double> dist_pos_x(0.,
                                                    graphic_par::window_width);
  std::uniform_real_distribution<double> dist_pos_y(0.,
                                                    graphic_par::window_height);

  std::uniform_real_distribution<double> dist_angle(0., 2 * M_PI);
  std::uniform_real_distribution<double> dist_speed_prey(prey_min_speed_,
                                                         prey_max_speed_);
  std::uniform_real_distribution<double> dist_speed_pred(predator_min_speed_,
                                                         predator_max_speed_);

  prey_flock_.clear();
  predator_flock_.clear();

  for (std::size_t i = 0; i < n_prey_; ++i) {
    double x = dist_pos_x(mt_);
    double y = dist_pos_y(mt_);
    point::Point pos(x, y);

    double angle = dist_angle(mt_);
    double speed = dist_speed_prey(mt_);
    point::Point vel(speed * std::cos(angle), speed * std::sin(angle));

    prey_flock_.emplace_back(std::make_shared<boid::Prey>(pos, vel));
  }
  assert(!prey_flock_.empty());

  for (std::size_t i = 0; i < n_predators_; ++i) {
    double x = dist_pos_x(mt_);
    double y = dist_pos_y(mt_);
    point::Point pos(x, y);

    double angle = dist_angle(mt_);
    double speed = dist_speed_pred(mt_);
    point::Point vel(speed * std::cos(angle), speed * std::sin(angle));

    predator_flock_.emplace_back(std::make_shared<boid::Predator>(pos, vel));
  }
  if (n_predators_ > 0) {
    assert(!predator_flock_.empty());
  }
}

std::vector<std::shared_ptr<boid::Boid>> Flock::findNearPrey(
    std::size_t i, bool is_prey) const {
  std::vector<std::shared_ptr<boid::Boid>> near_prey;

  if (is_prey) {
    double beta = prey_flock_[i]->getVelocity().angle();
    point::Point target_pos = prey_flock_[i]->getPosition();

    for (std::size_t j = 0; j < n_prey_; ++j) {
      if (i == j) continue;
      const auto& other_pos = prey_flock_[j]->getPosition();
      double distance = target_pos.distance(other_pos);
      if (distance < boids_dis_) {
        double alpha = (other_pos - target_pos).angle();
        double diff_angle = std::fmod(std::abs(alpha - beta), 2 * M_PI);
        if (diff_angle < prey_sight_angle_ ||
            diff_angle > 2 * M_PI - prey_sight_angle_) {
          near_prey.push_back(prey_flock_[j]);
        }
      }
    }
  } else {
    double beta = predator_flock_[i]->getVelocity().angle();
    point::Point target_pos = predator_flock_[i]->getPosition();

    for (size_t j = 0; j < n_prey_; ++j) {
      const auto& other_pos = prey_flock_[j]->getPosition();
      double distance = target_pos.distance(other_pos);
      if (distance < boids_dis_) {
        double alpha = (other_pos - target_pos).angle();
        double diff_angle = std::fmod(std::abs(alpha - beta), 2 * M_PI);
        if (diff_angle < predator_sight_angle_ ||
            diff_angle > 2 * M_PI - predator_sight_angle_) {
          near_prey.push_back(prey_flock_[j]);
        }
      }
    }
  }
  return near_prey;
}

std::vector<std::shared_ptr<boid::Boid>> Flock::findNearPredators(
    size_t i, bool is_prey) const {
  std::vector<std::shared_ptr<boid::Boid>> near_predators;

  if (is_prey) {
    double beta = prey_flock_[i]->getVelocity().angle();
    point::Point target_pos = prey_flock_[i]->getPosition();

    for (size_t j = 0; j < n_predators_; ++j) {
      const auto& other_pos = predator_flock_[j]->getPosition();
      double distance = target_pos.distance(other_pos);
      if (distance < boids_dis_) {
        double alpha = (other_pos - target_pos).angle();
        double diff_angle = std::fmod(std::abs(alpha - beta), 2 * M_PI);
        if (diff_angle < prey_sight_angle_ ||
            diff_angle > 2 * M_PI - prey_sight_angle_) {
          near_predators.push_back(predator_flock_[j]);
        }
      }
    }
  } else {
    double beta = predator_flock_[i]->getVelocity().angle();
    point::Point target_pos = predator_flock_[i]->getPosition();

    for (size_t j = 0; j < n_predators_; ++j) {
      if (i == j) continue;
      const auto& other_pos = predator_flock_[j]->getPosition();
      double distance = target_pos.distance(other_pos);
      if (distance < boids_dis_) {
        double alpha = (other_pos - target_pos).angle();
        double diff_angle = std::fmod(std::abs(alpha - beta), 2 * M_PI);
        if (diff_angle < predator_sight_angle_ ||
            diff_angle > 2 * M_PI - predator_sight_angle_) {
          near_predators.push_back(predator_flock_[j]);
        }
      }
    }
  }
  return near_predators;
}

std::array<point::Point, 2> Flock::updateBoid(sf::VertexArray& triangles,
                                              size_t i, bool is_prey) const {
  if (is_prey) {
    point::Point p = prey_flock_[i]->getPosition();

    const auto near_prey = findNearPrey(i, true);
    const auto near_predators = findNearPredators(i, true);

    point::Point v = prey_flock_[i]->border(boids_dis_, 0.05);

    if (!near_predators.empty()) {
      v += prey_flock_[i]->repel(flight_params_.repulsion, near_predators);
    }
    if (!near_prey.empty()) {
      v += prey_flock_[i]->separation(flight_params_.separation, prey_sep_,
                                      near_prey) +
           prey_flock_[i]->alignment(flight_params_.alignment, near_prey) +
           prey_flock_[i]->cohesion(flight_params_.cohesion, near_prey);
    }

    prey_flock_[i]->boost(speed_limits_.prey_min, v);
    prey_flock_[i]->friction(speed_limits_.prey_max, v);

    double theta = v.angle();
    p += graphic_par::dt * v;

    triangles::rotateTriangle(p, triangles, theta, i, true,
                              n_prey_ + n_predators_);
    return {p, v};
  } else {
    point::Point p = predator_flock_[i]->getPosition();

    const auto near_prey = findNearPrey(i, false);
    const auto near_predators = findNearPredators(i, false);

    point::Point v = predator_flock_[i]->border(boids_dis_, 0.05);

    if (!near_predators.empty()) {
      v += predator_flock_[i]->separation(flight_params_.separation,
                                          predator_sep_, near_predators);
    }
    if (!near_prey.empty()) {
      v += predator_flock_[i]->chase(flight_params_.chase, near_prey);
    }

    predator_flock_[i]->boost(speed_limits_.predator_min, v);
    predator_flock_[i]->friction(speed_limits_.predator_max, v);

    double theta = v.angle();
    p += graphic_par::dt * v;

    triangles::rotateTriangle(p, triangles, theta, i, false,
                              n_prey_ + n_predators_);
    return {p, v};
  }
}

void Flock::evolve(sf::VertexArray& triangles) const {
  std::vector<point::Point> prey_positions;
  std::vector<point::Point> prey_velocities;
  prey_positions.reserve(n_prey_);
  prey_velocities.reserve(n_prey_);

  std::vector<point::Point> predator_positions;
  std::vector<point::Point> predator_velocities;
  predator_positions.reserve(n_predators_);
  predator_velocities.reserve(n_predators_);

  for (size_t i = 0; i < n_prey_; ++i) {
    auto [pos, vel] = updateBoid(triangles, i, true);
    prey_positions.push_back(pos);
    prey_velocities.push_back(vel);
  }

  for (size_t i = 0; i < n_predators_; ++i) {
    auto [pos, vel] = updateBoid(triangles, i, false);
    predator_positions.push_back(pos);
    predator_velocities.push_back(vel);
  }

  for (size_t i = 0; i < n_prey_; ++i) {
    prey_flock_[i]->setPosition(prey_positions[i]);
    prey_flock_[i]->setVelocity(prey_velocities[i]);
  }
  for (size_t i = 0; i < n_predators_; ++i) {
    predator_flock_[i]->setPosition(predator_positions[i]);
    predator_flock_[i]->setVelocity(predator_velocities[i]);
  }
}

}  // namespace flock
