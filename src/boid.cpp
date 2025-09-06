#include "../include/boid.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

#include "../include/constants.hpp"

namespace boid {

// ---------- Boid ----------

Boid::Boid(const point::Point& position, const point::Point& velocity)
    : position_(position), velocity_(velocity) {}

point::Point Boid::getPosition() const { return position_; }
point::Point Boid::getVelocity() const { return velocity_; }

void Boid::setBoid(const point::Point& position, const point::Point& velocity) {
  position_ = position;
  velocity_ = velocity;
}

double Boid::angle(const Boid& other) const {
  const point::Point delta =
      point::relativePosition(position_, other.getPosition());
  const double vel_mag = velocity_.distance();
  const double delta_mag = delta.distance();

  if (vel_mag == 0.0 || delta_mag == 0.0) return 0.0;

  double cosine =
      (velocity_.getX() * delta.getX() + velocity_.getY() * delta.getY()) /
      (vel_mag * delta_mag);
  cosine = std::clamp(cosine, -1.0, 1.0);

  const double sign =
      velocity_.getX() * delta.getY() - velocity_.getY() * delta.getX();

  return (sign >= 0) ? std::acos(cosine) : -std::acos(cosine);
}

point::Point Boid::separation(
    const double s, const double ds,
    const std::vector<std::shared_ptr<Boid>>& near) const {
  assert(s >= 0 && s <= 1);
  assert(ds >= 0);

  const point::Point sum = std::accumulate(
      near.begin(), near.end(), point::Point(0., 0.),
      [this, ds](const point::Point& acc, const std::shared_ptr<Boid>& b) {
        if (point::toroidalDistance(position_, b->getPosition()) < ds) {
          return acc + point::relativePosition(position_, b->getPosition());
        }
        return acc;
      });

  return sum * (-s);
}

// ---------- Prey ----------

Prey::Prey() = default;
Prey::Prey(const point::Point& position, const point::Point& velocity)
    : Boid(position, velocity) {}

point::Point Prey::alignment(
    const double a, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(a >= 0 && a <= 1);
  if (near_prey.empty()) return {0., 0.};

  const point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [](const point::Point& acc, const std::shared_ptr<Boid>& b) {
        return acc + b->getVelocity();
      });

  return (sum / static_cast<double>(near_prey.size()) - velocity_) * a;
}

point::Point Prey::cohesion(
    const double c, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(c >= 0 && c <= 1);
  if (near_prey.empty()) return {0., 0.};

  const point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [this](const point::Point& acc, const std::shared_ptr<Boid>& b) {
        return acc + point::relativePosition(position_, b->getPosition());
      });

  return (sum / static_cast<double>(near_prey.size())) * c;
}

point::Point Prey::repulsion(
    const double r,
    const std::vector<std::shared_ptr<Boid>>& near_predators) const {
  assert(r >= 0);
  if (near_predators.empty()) return {0., 0.};

  const point::Point sum = std::accumulate(
      near_predators.begin(), near_predators.end(), point::Point(0., 0.),
      [this](const point::Point& acc, const std::shared_ptr<Boid>& b) {
        return acc + point::relativePosition(position_, b->getPosition());
      });

  return sum * (-r);
}

void Prey::clamp(const double min_speed, const double max_speed,
                 point::Point& velocity) {
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  const double speed = velocity.distance();
  if (speed > max_speed)
    velocity = (velocity / speed) * max_speed;
  else if (speed < min_speed)
    velocity = (velocity / speed) * min_speed;
}

// ---------- Predator ----------

Predator::Predator() = default;
Predator::Predator(const point::Point& position, const point::Point& velocity)
    : Boid(position, velocity) {}

point::Point Predator::chase(
    const double ch,
    const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(ch >= 0 && ch <= 1);
  if (near_prey.empty()) return {0., 0.};

  const point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [this](const point::Point& acc, const std::shared_ptr<Boid>& b) {
        return acc + point::relativePosition(position_, b->getPosition());
      });

  return (sum / static_cast<double>(near_prey.size())) * ch;
}

void Predator::clamp(const double min_speed, const double max_speed,
                     point::Point& velocity) {
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  const double speed = velocity.distance();
  if (speed > max_speed)
    velocity = (velocity / speed) * max_speed;
  else if (speed < min_speed)
    velocity = (velocity / speed) * min_speed;
}

}  // namespace boid
