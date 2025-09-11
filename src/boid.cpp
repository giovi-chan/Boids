#include "../include/boid.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

#include "../include/graphics.hpp"

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
  assert(s >= 0);
  assert(ds >= 0);
  if (near.empty()) {
    return {0., 0.};
  }
  const point::Point sum = std::accumulate(
      near.begin(), near.end(), point::Point(0., 0.),
      [this, ds](const point::Point& accumulate,
                 const std::shared_ptr<Boid>& boid) {
        if (point::toroidalDistance(position_, boid->getPosition()) < ds) {
          return accumulate +
                 point::relativePosition(position_, boid->getPosition());
        }
        return accumulate;
      });

  return (-s) * sum;
}

// ---------- Prey ----------

Prey::Prey() = default;
Prey::Prey(const point::Point& position, const point::Point& velocity)
    : Boid(position, velocity) {}

point::Point Prey::alignment(
    const double a, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(a >= 0);
  if (near_prey.empty()) {
    return {0., 0.};
  }

  const point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [](const point::Point& accumulate, const std::shared_ptr<Boid>& boid) {
        return accumulate + boid->getVelocity();
      });

  return a * (sum / static_cast<double>(near_prey.size()) - velocity_);
}

point::Point Prey::cohesion(
    const double c, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(c >= 0);
  if (near_prey.empty()) {
    return {0., 0.};
  }

  const point::Point sum =
      std::accumulate(near_prey.begin(), near_prey.end(), point::Point(0., 0.),
                      [this](const point::Point& accumulate,
                             const std::shared_ptr<Boid>& boid) {
                        return accumulate + point::relativePosition(
                                                position_, boid->getPosition());
                      });

  return c * (sum / static_cast<double>(near_prey.size()));
}

point::Point Prey::repulsion(
    const double r,
    const std::vector<std::shared_ptr<Boid>>& near_predators) const {
  assert(r >= 0);
  if (near_predators.empty()) {
    return {0., 0.};
  }

  const point::Point sum = std::accumulate(
      near_predators.begin(), near_predators.end(), point::Point(0., 0.),
      [this](const point::Point& accumulate,
             const std::shared_ptr<Boid>& boid) {
        return accumulate +
               point::relativePosition(position_, boid->getPosition());
      });

  return (-r) * sum;
}

void Prey::clamp(const double min_speed, const double max_speed,
                 point::Point& velocity) {
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  const double speed = velocity.distance();
  if (speed == 0.0) {
    velocity = {min_speed, 0.};
    return;
  }
  if (speed > max_speed)
    velocity = max_speed * (velocity / speed);
  else if (speed < min_speed)

    velocity = min_speed * (velocity / speed);
}

// ---------- Predator ----------

Predator::Predator() = default;
Predator::Predator(const point::Point& position, const point::Point& velocity)
    : Boid(position, velocity) {}

point::Point Predator::chase(
    const double ch,
    const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(ch >= 0);
  if (near_prey.empty()) return {0., 0.};

  const point::Point sum =
      std::accumulate(near_prey.begin(), near_prey.end(), point::Point(0., 0.),
                      [this](const point::Point& accumulate,
                             const std::shared_ptr<Boid>& boid) {
                        return accumulate + point::relativePosition(
                                                position_, boid->getPosition());
                      });

  return ch * sum;
}

void Predator::clamp(const double min_speed, const double max_speed,
                     point::Point& velocity) {
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  const double speed = velocity.distance();
  if (speed == 0.0) {
    velocity = {min_speed, 0.};
    return;
  }
  if (speed > max_speed)
    velocity = max_speed * (velocity / speed);
  else if (speed < min_speed)
    velocity = min_speed * (velocity / speed);
}

}  // namespace boid
