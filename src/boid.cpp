#include "../include/boid.hpp"

#include <cassert>

namespace boid{
// Constructor
Boid::Boid(point::Point position, point::Point velocity)
    : position_{position}, velocity_{velocity} {}

// getters
point::Point Boid::position() const { return position_; }
point::Point Boid::velocity() const { return velocity_; }

// flight rules
point::Point Boid::alignment(const std::vector<Boid>& neighbors,
                             double alignment_coeff) const {
  if (neighbors.empty()) return point::Point{0, 0};

  point::Point avg_velocity{0, 0};
  for (const auto& neighbor : neighbors) {
    avg_velocity = avg_velocity + neighbor.velocity();
  }
  avg_velocity = avg_velocity * (1.0 / neighbors.size());

  return (avg_velocity - velocity_) * alignment_coeff;
}

point::Point Boid::separation(const std::vector<Boid>& neighbors,
                              double separation_dist,
                              double separation_coeff) const {
  point::Point force{0., 0.};
  for (const auto& neighbor : neighbors) {
    double dist = (position_ - neighbor.position()).distance();
    if (dist < separation_dist) {
      force = force + neighbor.position() - position_;
    }
  }
  return force * separation_coeff * (-1.);
}

point::Point Boid::cohesion(const std::vector<Boid>& neighbors,
                            double cohesion_coeff) const {
  if (neighbors.empty()) return point::Point{0, 0};

  point::Point center_of_mass{0, 0};
  for (const auto& neighbor : neighbors) {
    center_of_mass = center_of_mass + neighbor.position();
  }
  center_of_mass = center_of_mass * (1.0 / neighbors.size());

  return (center_of_mass - position_) * cohesion_coeff;
}

void Boid::update(double delta_t, const std::vector<Boid>& neighbors,
                  double separation_dist, double separation_coeff,
                  double cohesion_coeff, double alignment_coeff) {
  assert(delta_t >= 0);

  point::Point separation_force =
      separation(neighbors, separation_dist, separation_coeff);
  point::Point cohesion_force = cohesion(neighbors, cohesion_coeff);
  point::Point alignment_force = alignment(neighbors, alignment_coeff);

  velocity_ = velocity_ + separation_force + cohesion_force + alignment_force;

  const double max_speed = 10.0;
  if (velocity_.distance() > max_speed) {
    velocity_ = velocity_ * (max_speed / velocity_.distance());
  }

  position_ = position_ + velocity_ * delta_t;
}
}
