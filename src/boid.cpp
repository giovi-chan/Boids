#include "../include/boid.hpp"

#include <cassert>

#include "../include/constants.hpp"

namespace boid {
// Constructor
Boid::Boid(point::Point position, point::Point velocity)
    : position_{position}, velocity_{velocity} {}

// getters
point::Point Boid::get_position() const { return position_; }
point::Point Boid::get_velocity() const { return velocity_; }

point::Point relative_position(point::Point const& p1, point::Point const& p2) {
  double delta_x{p2.get_x() - p1.get_x()};
  double delta_y{p2.get_y() - p1.get_x()};

  if (delta_x > static_cast<double>(constants::window_width / 2.)) {
    delta_x -= constants::window_width;
    assert(delta_x < 0.);
  } else if (delta_x < static_cast<double>(constants::window_width / (-2.))) {
    delta_x += constants::window_width;
    assert(delta_x > 0.);
  }

  if (delta_y > static_cast<double>(constants::window_height / 2.)) {
    delta_y -= constants::window_height;
    assert(delta_y < 0.);
  } else if (delta_y < static_cast<double>(constants::window_height / (-2.))) {
    delta_y += constants::window_height;
    assert(delta_y > 0.);
  }
  return {delta_x, delta_y};
}

// flight rules
point::Point Boid::alignment(const std::vector<Boid>& neighbors,
                             double alignment_coeff) const {
  if (neighbors.empty()) return point::Point{0., 0.};
  assert(neighbors.size() !=0);
  point::Point avg_velocity{0., 0.};
  for (const auto& neighbor : neighbors) {
    avg_velocity = avg_velocity + neighbor.get_velocity();
  }
  avg_velocity = avg_velocity / static_cast<double>(neighbors.size());

  return (avg_velocity - velocity_) * alignment_coeff;
}

point::Point Boid::separation(const std::vector<Boid>& neighbors,
                              double separation_dist,
                              double separation_coeff) const {
  point::Point force{0., 0.};
  for (const auto& neighbor : neighbors) {
    double dist = relative_position(position_, neighbor.get_position()).distance();
    if (dist < separation_dist) {
      force = force - neighbor.get_position() + position_;
    }
  }
  return force * separation_coeff ;
}

point::Point Boid::cohesion(const std::vector<Boid>& neighbors,
                            double cohesion_coeff) const {
  if (neighbors.empty()) return point::Point{0., 0.};

  point::Point center_of_mass{0, 0};
  for (const auto& neighbor : neighbors) {
    center_of_mass = center_of_mass + relative_position(neighbor.get_position(), {0.,0.});
  }
  center_of_mass = center_of_mass / static_cast<double>(neighbors.size());

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


  if (velocity_.distance() > constants::max_velocity) {
    velocity_ = velocity_ * (constants::max_velocity / velocity_.distance());
  }

  position_ = position_ + velocity_ * delta_t;

  // Effetto Pac-Man (wrap around)
  if (position_.get_x() < 0)
    position_.set_x(position_.get_x() + constants::window_width);
  else if (position_.get_x() >= constants::window_width)
    position_.set_x(position_.get_x() - constants::window_width);

  if (position_.get_y() < 0)
    position_.set_y(position_.get_y() + constants::window_height);
  else if (position_.get_y() >= constants::window_height)
    position_.set_y(position_.get_y() - constants::window_height);
}
}  // namespace boid
