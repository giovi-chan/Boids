#include "../include/boid.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>  // per std::accumulate

#include "../include/constants.hpp"

namespace boid {

// ---------- Boid

Boid::Boid(point::Point const& position, point::Point const& velocity)
    : position_(position), velocity_(velocity) {}

point::Point Boid::getPosition() const { return position_; }
point::Point Boid::getVelocity() const { return velocity_; }

void Boid::setBoid(point::Point position, point::Point velocity) {
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
  cosine = std::clamp(cosine, -1.0, 1.0);

  return std::acos(cosine);
}

point::Point Boid::separation(
    double s, double d, const std::vector<std::shared_ptr<Boid>>& near) const {
  assert(s >= 0 && s <= 1);
  assert(d >= 0);

  point::Point sum = std::accumulate(
      near.begin(), near.end(), point::Point(0., 0.),
      [this, d](point::Point accumulate, const std::shared_ptr<Boid>& boid) {
        // Assumendo che toroidal_distance sia una funzione globale accessibile
        if (toroidalDistance(position_, boid->getPosition()) < d) {
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
    double a, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(a >= 0 && a <= 1);
  if (near_prey.empty()) return point::Point(0., 0.);

  point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [](point::Point accumulate, const std::shared_ptr<Boid>& boid) {
        return accumulate + boid->getVelocity();
      });
  return a * (sum / static_cast<double>(near_prey.size()) - velocity_);
}

point::Point Prey::cohesion(
    double c, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(c >= 0 && c <= 1);
  if (near_prey.empty()) return point::Point(0., 0.);

  point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [this](point::Point accumulate, const std::shared_ptr<Boid>& boid) {
        return accumulate +
               point::relativePosition(position_, boid->getPosition());
      });
  return c * (sum / static_cast<double>(near_prey.size()));
}

point::Point Prey::repulsion(
    double r, const std::vector<std::shared_ptr<Boid>>& near_predators) const {
  assert(r >= 0);
  if (near_predators.empty()) return point::Point(0., 0.);

  point::Point sum = std::accumulate(
      near_predators.begin(), near_predators.end(), point::Point(0., 0.),
      [this](point::Point accumulate, const std::shared_ptr<Boid>& boid) {
        return accumulate +
               point::relativePosition(position_, boid->getPosition());
      });
  return -r * sum;
}

void Prey::normalized(const double min_speed, const double max_speed,
                      point::Point& velocity) {
  assert(velocity.module() != 0);
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  double speed = velocity.module();

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
    double ch, const std::vector<std::shared_ptr<Boid>>& near_prey) const {
  assert(ch >= 0 && ch <= 1);
  if (near_prey.empty()) return point::Point(0., 0.);

  point::Point sum = std::accumulate(
      near_prey.begin(), near_prey.end(), point::Point(0., 0.),
      [this](point::Point accumulate, const std::shared_ptr<Boid>& boid) {
        return accumulate +
               point::relativePosition(position_, boid->getPosition());
      });

  return ch * (sum / static_cast<double>(near_prey.size()));
}

void Predator::normalized(const double min_speed, const double max_speed,
                          point::Point& velocity) {
  assert(velocity.module() != 0);
  assert(min_speed >= 0);
  assert(max_speed > 0);
  assert(min_speed <= max_speed);

  double speed = velocity.module();

  if (speed > max_speed) {
    velocity = max_speed * (velocity / speed);
  } else if (speed < min_speed) {
    velocity = min_speed * (velocity / speed);
  }
}

}  // namespace boid
