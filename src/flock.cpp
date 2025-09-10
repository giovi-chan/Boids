#include "../include/flock.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

#include "../include/boid.hpp"
#include "../include/graphics.hpp"
#include "../include/point.hpp"
#include "../include/statistics.hpp"

namespace flock {

Flock::Flock(const std::size_t nPrey, const std::size_t nPredators)
    : n_prey_(nPrey),
      n_predators_(nPredators),
      flight_parameters_{0.1, 0.1, 0.004, 0.6, 0.008},
      speed_limits_{7., 12., 5., 8.} {
  prey_flock_.reserve(n_prey_);
  predator_flock_.reserve(n_predators_);
}

Flock::Flock(const std::vector<std::shared_ptr<boid::Prey>>& prey,
             const std::vector<std::shared_ptr<boid::Predator>>& predators,
             const SpeedLimits& speed_limits)
    : n_prey_(prey.size()),
      n_predators_(predators.size()),
      prey_flock_(prey),
      predator_flock_(predators),
      flight_parameters_{0.1, 0.1, 0.004, 0.6, 0.008},
      speed_limits_(speed_limits) {}

std::size_t Flock::getPreyNum() const { return n_prey_; }
std::size_t Flock::getPredatorsNum() const { return n_predators_; }
std::size_t Flock::getFlockSize() const { return n_prey_ + n_predators_; }

std::vector<std::shared_ptr<boid::Prey>> Flock::getPreyFlock() const {
  return prey_flock_;
}
std::vector<std::shared_ptr<boid::Predator>> Flock::getPredatorFlock() const {
  return predator_flock_;
}

FlightParameters Flock::getFlightParameters() const {
  return flight_parameters_;
}

std::array<double, 3> Flock::getDistancesParameters() {
  return {d_, prey_ds_, predator_ds_};
}

void Flock::setFlockSize() {
  std::cout << "Enter the number of prey to simulate: ";
  std::size_t prey;
  std::cin >> prey;
  if (std::cin.fail() || prey == 0 || prey > 1000) {
    std::cout << "\nInvalid input, using default value.";
    prey = 200;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  std::cout << "\nEnter the number of predators to simulate: ";
  std::size_t predators;
  std::cin >> predators;
  if (std::cin.fail() || predators > 100) {
    std::cout << "\n Invalid input, using default value.";
    predators = 5;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  n_prey_ = prey;
  n_predators_ = predators;
}

void Flock ::setFlightParameters() {
  std::cout << "\nWould you like to customize the parameters of the simulation?"
               "\n (Y/n)";
  char statement;
  std::cin >> statement;
  if (std::cin.fail() || (statement != 'Y' && statement != 'y' &&
                          statement != 'N' && statement != 'n')) {
    std::cout << "\nInvalid input, using default values.\n separation=0.1\n "
                 "alignment=0.1\n "
                 "cohesion=0.004\n repulsion=0.6\n chase=0.008\n";
    return;
  } else if (statement == 'Y' || statement == 'y') {
    double s;
    std::cout << "Enter separation coefficient: ";
    std::cin >> s;
    double a;
    std::cout << "Enter alignment coefficient: ";
    std::cin >> a;
    double c;
    std::cout << "Enter cohesion coefficient: ";
    std::cin >> c;
    if (std::cin.fail()) {
      std::cout << "\nInvalid input, using default values.\n separation=0.1\n "
                   "alignment=0.1\n "
                   "cohesion=0.004\n repulsion=0.6\n chase=0.008\n";
      return;
    }

    flight_parameters_.separation = s;
    flight_parameters_.alignment = a;
    flight_parameters_.cohesion = c;

    flight_parameters_.repulsion = s * 6;
    flight_parameters_.chase = c * 2;
  } else {
    std::cout << "\nUsing default values.\n separation=0.1\n alignment=0.1\n "
                 "cohesion=0.004\n "
                 "repulsion=0.6\n chase=0.008 \n";
  }
}

void Flock::generateBoids() {
  std::uniform_real_distribution<> dist_pos_x(0., graphics::window_width);
  std::uniform_real_distribution<> dist_pos_y(0., graphics::window_height);
  std::uniform_real_distribution<> dist_angle(0, 2 * M_PI);
  std::uniform_real_distribution<> dist_vel(0, 5.);

  prey_flock_.clear();
  prey_flock_.reserve(n_prey_);

  for (std::size_t i = 0; i < n_prey_; ++i) {
    prey_flock_.emplace_back(std::make_shared<boid::Prey>(
        point::Point(dist_pos_x(mt_), dist_pos_y(mt_)),
        point::Point(dist_vel(mt_) * std::cos(dist_angle(mt_)),
                     dist_vel(mt_) * std::sin(dist_angle(mt_)))));
  }

  predator_flock_.clear();
  predator_flock_.reserve(n_predators_);

  for (std::size_t i = 0; i < n_predators_; ++i) {
    predator_flock_.emplace_back(std::make_shared<boid::Predator>(
        point::Point(dist_pos_x(mt_), dist_pos_y(mt_)),
        point::Point(dist_vel(mt_) * std::cos(dist_angle(mt_)),
                     dist_vel(mt_) * std::sin(dist_angle(mt_)))));
  }
}

std::vector<std::shared_ptr<boid::Boid>> Flock::nearPrey(
    const std::size_t i, const bool is_prey) const {
  std::vector<std::shared_ptr<boid::Boid>> near;

  std::shared_ptr<boid::Boid> target =
      is_prey ? std::static_pointer_cast<boid::Boid>(prey_flock_[i])
              : std::static_pointer_cast<boid::Boid>(predator_flock_[i]);

  for (std::size_t j = 0; j < n_prey_; ++j) {
    if (is_prey && i == j) continue;

    const auto& other = prey_flock_[j];

    double dist =
        point::toroidalDistance(target->getPosition(), other->getPosition());

    if (dist < d_) {
      double relative_angle = target->angle(*other);

      double sight_angle = is_prey ? prey_sight_angle_ : predator_sight_angle_;
      if (std::abs(relative_angle) < sight_angle) {
        near.emplace_back(other);
      }
    }
  }

  return near;
}

std::vector<std::shared_ptr<boid::Boid>> Flock::nearPredators(
    const std::size_t i, const bool is_prey) const {
  std::vector<std::shared_ptr<boid::Boid>> near;

  std::shared_ptr<boid::Boid> target =
      is_prey ? std::static_pointer_cast<boid::Boid>(prey_flock_[i])
              : std::static_pointer_cast<boid::Boid>(predator_flock_[i]);

  for (std::size_t j = 0; j < n_predators_; ++j) {
    if (!is_prey && i == j) continue;

    const auto& other = predator_flock_[j];

    const double dist =
        point::toroidalDistance(target->getPosition(), other->getPosition());

    if (dist < d_) {
      const double relative_angle = target->angle(*other);

      const double sight_angle =
          is_prey ? prey_sight_angle_ : predator_sight_angle_;
      if (std::abs(relative_angle) < sight_angle) {
        near.emplace_back(other);
      }
    }
  }

  return near;
}

std::array<point::Point, 2> Flock::updateBoid(std::size_t i, bool is_prey,
                                              const double dt) const {
  point::Point pos;
  point::Point vel;

  if (is_prey) {
    pos = prey_flock_[i]->getPosition();
    vel = prey_flock_[i]->getVelocity();

    auto near_prey = nearPrey(i, true);
    auto near_predators = nearPredators(i, true);

    if (!near_predators.empty())
      vel += prey_flock_[i]->repulsion(flight_parameters_.repulsion,
                                       near_predators);

    if (!near_prey.empty())
      vel +=
          prey_flock_[i]->separation(flight_parameters_.separation, prey_ds_,
                                     near_prey) +
          prey_flock_[i]->alignment(flight_parameters_.alignment, near_prey) +
          prey_flock_[i]->cohesion(flight_parameters_.cohesion, near_prey);

    prey_flock_[i]->clamp(speed_limits_.prey_min, speed_limits_.prey_max, vel);

  } else {
    pos = predator_flock_[i]->getPosition();
    vel = predator_flock_[i]->getVelocity();

    auto near_prey = nearPrey(i, false);
    auto near_predators = nearPredators(i, false);

    if (!near_predators.empty())
      vel += predator_flock_[i]->separation(flight_parameters_.separation,
                                            predator_ds_, near_predators);

    if (!near_prey.empty())
      vel += predator_flock_[i]->chase(flight_parameters_.chase, near_prey);

    predator_flock_[i]->clamp(speed_limits_.predator_min,
                              speed_limits_.predator_max, vel);
  }

  pos += vel * dt;

  if (pos.getX() < 0) {
    pos.setX(pos.getX() + graphics::window_width);
  }
  if (pos.getX() > graphics::window_width) {
    pos.setX(pos.getX() - graphics::window_width);
  }
  if (pos.getY() < 0) {
    pos.setY(pos.getY() + graphics::window_height);
  }
  if (pos.getY() > graphics::window_height) {
    pos.setY(pos.getY() - graphics::window_height);
  }

  return {pos, vel};
}

void Flock::updateFlock(const double dt) const {
  std::vector<point::Point> new_prey_pos;
  std::vector<point::Point> new_prey_vel;
  new_prey_pos.reserve(n_prey_);
  new_prey_vel.reserve(n_prey_);

  std::vector<point::Point> new_pred_pos;
  std::vector<point::Point> new_pred_vel;
  new_pred_pos.reserve(n_predators_);
  new_pred_vel.reserve(n_predators_);

  for (std::size_t i = 0; i < n_prey_; ++i) {
    auto result = updateBoid(i, true, dt);
    new_prey_pos.push_back(result[0]);
    new_prey_vel.push_back(result[1]);
  }

  for (std::size_t i = 0; i < n_predators_; ++i) {
    auto result = updateBoid(i, false, dt);
    new_pred_pos.push_back(result[0]);
    new_pred_vel.push_back(result[1]);
  }

  for (std::size_t i = 0; i < n_predators_; ++i) {
    predator_flock_[i]->setBoid(new_pred_pos[i], new_pred_vel[i]);
  }
  for (std::size_t i = 0; i < n_prey_; ++i) {
    prey_flock_[i]->setBoid(new_prey_pos[i], new_prey_vel[i]);
  }
}

statistics::Statistics Flock::statistics() const {
  const int n = static_cast<int>(n_prey_);

  double mean_dist = 0.0;
  double mean_dist2 = 0.0;
  for (auto it = prey_flock_.begin(); it != prey_flock_.begin() + n; ++it) {
    auto sum = std::accumulate(
        it + 1, prey_flock_.begin() + n, std::array<double, 2>{0.0, 0.0},
        [&it](auto& accumulate, const std::shared_ptr<boid::Boid>& boid) {
          const double dist = point::toroidalDistance((*it)->getPosition(),
                                                      boid->getPosition());
          accumulate[0] += dist;
          accumulate[1] += dist * dist;
          return accumulate;
        });
    mean_dist += sum[0];
    mean_dist2 += sum[1];
  }
  const double denom = n * (n - 1) / 2.0;  // numero di coppie uniche i<j
  mean_dist /= denom;
  mean_dist2 /= denom;

  // Calcolo media e deviazione standard della velocità
  auto sum = std::accumulate(
      prey_flock_.begin(), prey_flock_.begin() + n,
      std::array<double, 2>{0.0, 0.0},
      [](auto& accumulate, const std::shared_ptr<boid::Boid>& boid) {
        const double vel = boid->getVelocity().distance();
        accumulate[0] += vel;
        accumulate[1] += vel * vel;
        return accumulate;
      });

  const double mean_speed = sum[0] / n;
  const double mean_speed2 = sum[1] / n;

  const double dev_dist = std::sqrt(mean_dist2 - mean_dist * mean_dist);
  const double dev_speed = std::sqrt(mean_speed2 - mean_speed * mean_speed);

  return {mean_dist, dev_dist, mean_speed, dev_speed};
}

}  // namespace flock
