#include "../include/flock.hpp"

#include <SFML/Graphics/VertexArray.hpp>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
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

std::vector<std::shared_ptr<boid::Boid>> Flock::nearPrey(
    const std::size_t i, const bool is_prey) const {
  std::vector<std::shared_ptr<boid::Boid>> near_prey;

  const point::Point my_position = is_prey ? prey_flock_[i]->getPosition()
                                           : predator_flock_[i]->getPosition();

  for (std::size_t j = 0; j < n_prey_; ++j) {
    if (is_prey && i == j) continue;  // Evita autoconfronto

    const auto& other = prey_flock_[j];
    const point::Point other_position = other->getPosition();

    if (toroidal_distance(my_position, other_position) < boids_dis_) {
      double alpha = is_prey ? prey_flock_[i]->angle(other)
                             : predator_flock_[i]->angle(other);

      if (std::abs(alpha) <
          (is_prey ? prey_sight_angle_ : predator_sight_angle_)) {
        near_prey.emplace_back(other);
      }
    }
  }

  return near_prey;
}

std::vector<std::shared_ptr<boid::Boid>> Flock::nearPredators(
    const std::size_t i, const bool is_prey) const {
  std::vector<std::shared_ptr<boid::Boid>> near_predators;

  const point::Point my_position = is_prey ? prey_flock_[i]->getPosition()
                                           : predator_flock_[i]->getPosition();

  for (std::size_t j = 0; j < n_predators_; ++j) {
    if (!is_prey && i == j) continue;

    const auto& other = predator_flock_[j];
    const point::Point other_position = other->getPosition();

    if (toroidal_distance(my_position, other_position) < boids_dis_) {
      double alpha = is_prey ? prey_flock_[i]->angle(other)
                             : predator_flock_[i]->angle(other);

      if (std::abs(alpha) <
          (is_prey ? prey_sight_angle_ : predator_sight_angle_)) {
        near_predators.emplace_back(other);
      }
    }
  }

  return near_predators;
}

std::pair<point::Point, point::Point> Flock::updateBoid(std::size_t i,
                                                        bool is_prey) const {
  point::Point p, v;

  if (is_prey) {
    p = prey_flock_[i]->getPosition();
    v = prey_flock_[i]->getVelocity();

    const auto near_prey = findNearPrey(i, true);
    const auto near_predators = findNearPredators(i, true);

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
  } else {
    p = predator_flock_[i]->getPosition();
    v = predator_flock_[i]->border(boids_dis_, 0.05);

    const auto near_prey = findNearPrey(i, false);
    const auto near_predators = findNearPredators(i, false);

    if (!near_predators.empty()) {
      v += predator_flock_[i]->separation(flight_params_.separation,
                                          predator_sep_, near_predators);
    }
    if (!near_prey.empty()) {
      v += predator_flock_[i]->chase(flight_params_.chase, near_prey);
    }

    predator_flock_[i]->boost(speed_limits_.predator_min, v);
    predator_flock_[i]->friction(speed_limits_.predator_max, v);
  }

  p += graphic_par::dt * v;
  return {p, v};
}

void Flock::updateFlock() const {
  std::vector<point::Point> prey_pos;
  std::vector<point::Point> prey_vel;

  for (size_t i = 0; i < n_boids_; ++i) {
    // Evaluates new positions and velocities for each bird::Boid
    std::pair<point::Point, point::Point> prey = updateBoid(i, true);
    prey_pos.push_back(prey[0]);
    prey_vel.push_back(prey[1]);
  }

  if (n_predators_ > 0) {
    std::vector<point::Point> predator_pos;
    std::vector<point::Point> predator_vel;

    for (std::size_t i = 0; i < n_predators_; ++i) {
      std::array<point::Point, 2> predator = updateBoid(i, false);
      predator_pos.push_back(predator[0]);
      predator_vel.push_back(predator[1]);
    }

    for (std::size_t i = 0; i < n_predators_; ++i) {
      predator_flock_[i]->setBoid(predator_pos[i], predator_vel[i]);
    }
  }

  for (std::size_t i = 0; i < n_prey_; ++i) {
    prey_flock_[i]->setBoid(prey_pos[i], prey_vel[i]);
  }
}

statistics::Statistics Flock::statistics() const {
  double meanBoids_dist{0.};
  double meanBoids_dist2{0.};
  double dev_dist{0.};
  const int nBoids{static_cast<int>(n_boids_)};

  if (nBoids > 1) {
    for (auto it = b_flock_.begin(); it != b_flock_.begin() + nBoids; ++it) {
      std::array<double, 2> sum = std::accumulate(
          it, b_flock_.begin() + nBoids, std::array<double, 2>{0., 0.},
          [&it](std::array<double, 2>& acc,
                const std::shared_ptr<bird::Bird>& bird) {
            acc[0] += (bird->getPosition().distance((*it)->getPosition()));
            acc[1] += bird->getPosition().distance((*it)->getPosition()) *
                      bird->getPosition().distance((*it)->getPosition());
            return acc;
          });
      meanBoids_dist += sum[0];
      meanBoids_dist2 += sum[1];
    }
    const double denominator{nBoids * (nBoids - 1) / 2.};

    meanBoids_dist /= denominator;
    meanBoids_dist2 /= denominator;
    dev_dist = std::sqrt(meanBoids_dist2 - meanBoids_dist * meanBoids_dist);
  }

  double meanBoids_speed{0.};
  double meanBoids_speed2{0.};

  const std::array<double, 2> sum = std::accumulate(
      b_flock_.begin(), b_flock_.begin() + nBoids,
      std::array<double, 2>{0., 0.},
      [](std::array<double, 2>& acc, const std::shared_ptr<bird::Bird>& bird) {
        acc[0] += bird->getVelocity().module();
        acc[1] += bird->getVelocity().module() * bird->getVelocity().module();
        return acc;
      });
  assert(nBoids > 0 && "Flock must contain at least one element");
  meanBoids_speed = sum[0] / nBoids;
  meanBoids_speed2 = sum[1] / nBoids;

  const double dev_speed =
      std::sqrt(meanBoids_speed2 - meanBoids_speed * meanBoids_speed);

  return {meanBoids_dist, dev_dist, meanBoids_speed, dev_speed};
}
}  // namespace flock