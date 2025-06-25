#include "../include/boid.hpp"

#include <algorithm>  // for std::clamp
#include <cassert>
#include <cmath>    // for std::acos
#include <numeric>  // for std::accumulate

#include "../include/constants.hpp"

namespace boid {

// ---------- Boid

Boid::Boid(point::Point const& position, point::Point const& velocity)
    : position_(position), velocity_(velocity) {}

point::Point Boid::getPosition() const { return position_; }
point::Point Boid::getVelocity() const { return velocity_; }

void Boid::setBoid(const point::Point& position, const point::Point& velocity) {
  position_ = position;
  velocity_ = velocity;
}

double Boid::angle(const Boid& other) const {
  point::Point delta = point::relativePosition(position_, other.getPosition());
  double vel_mag = velocity_.distance();
  double delta_mag = delta.distance();

  if (vel_mag == 0.0 || delta_mag == 0.0) return 0.0;

  double cosine =
      (velocity_.getX() * delta.getX() + velocity_.getY() * delta.getY()) /
      (vel_mag * delta_mag);
  cosine = std::clamp(cosine, -1.0, 1.0);  // to check

  return std::acos(cosine);  // given a number between -1 an +1, returns the
                             // angle between -pi and +pi
}

point::Point Boid::separation(
    const double s, const double ds,
    const std::vector<std::shared_ptr<Boid>>& near) const {
  assert(s >= 0 && s <= 1);
  assert(ds >= 0);

  const point::Point sum = std::accumulate(
      near.begin(), near.end(), point::Point(0., 0.),
      [this, ds](point::Point accumulate, const std::shared_ptr<Boid>& boid) {
        if (point::toroidalDistance(position_, boid->getPosition()) < ds) {
          accumulate += point::relativePosition(position_, boid->getPosition());
        }
        return accumulate;
      });
  return -s * sum;
}

//----------------------- Prey

Prey::Prey() : Boid() {}
Prey::Prey(point::Point const& position, point::Point const& velocity)
    : Boid(position, velocity) {}

point::Point Prey::alignment(
    const double a, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(a >= 0 && a <= 1);
  if (near_prey.empty()) return point::Point(0., 0.);

  const point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [](const point::Point& accumulate, const std::shared_ptr<Boid>& boid) {
        return accumulate + boid->getVelocity();
      });

  return a * (sum / static_cast<double>(near_prey.size()) - velocity_);
}

point::Point Prey::cohesion(
    const double c, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(c >= 0 && c <= 1);
  if (near_prey.empty()) return point::Point(0., 0.);

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
  if (near_predators.empty()) return point::Point(0., 0.);

  const point::Point sum = std::accumulate(
      near_predators.begin(), near_predators.end(), point::Point(0., 0.),
      [this](const point::Point& accumulate,
             const std::shared_ptr<Boid>& boid) {
        return accumulate +
               point::relativePosition(position_, boid->getPosition());
      });
  return -r * sum;
}

void Prey::clamp(const double min_speed, const double max_speed,
                 point::Point& velocity) {
  assert(velocity.distance() != 0);
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  double speed = velocity.distance();

  if (speed > max_speed) {
    velocity = max_speed * (velocity / speed);
  } else if (speed < min_speed) {
    velocity = min_speed * (velocity / speed);
  }
}

//----------------------- Predator

Predator::Predator() : Boid() {}

Predator::Predator(point::Point const& position, point::Point const& velocity)
    : Boid(position, velocity) {}

point::Point Predator::chase(
    const double ch,
    const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(ch >= 0 && ch <= 1);
  if (near_prey.empty()) return point::Point(0., 0.);

  const point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [this](point::Point accumulate, const std::shared_ptr<Boid>& boid) {
        return accumulate +
               point::relativePosition(position_, boid->getPosition());
      });

  return ch * (sum / static_cast<double>(near_prey.size()));
}

void Predator::clamp(const double min_speed, const double max_speed,
                     point::Point& velocity) {
  assert(velocity.distance() != 0);
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  double speed = velocity.distance();

  if (speed > max_speed) {
    velocity = max_speed * (velocity / speed);
  } else if (speed < min_speed) {
    velocity = min_speed * (velocity / speed);
  }
}

}  // namespace boid
