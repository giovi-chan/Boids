
#include "../include/boid.hpp"

#include <cassert>
#include <cmath>

#include "../include/constants.hpp"

namespace boid {

// --- Boid ---

Boid::Boid(point::Point position, point::Point velocity)
    : position_{position}, velocity_{velocity} {}

point::Point Boid::get_position() const { return position_; }
point::Point Boid::get_velocity() const { return velocity_; }

void Boid::set_position(const point::Point& p) { position_ = p; }
void Boid::set_velocity(const point::Point& v) { velocity_ = v; }

double Boid::angle(const Boid& other) const {
  point::Point delta =
      point::relative_position(position_, other.get_position());
  double vel_mag = velocity_.distance();
  double delta_mag = delta.distance();

  if (vel_mag == 0.0 || delta_mag == 0.0) return 0.0;

  double cosine =
      (velocity_.get_x() * delta.get_x() + velocity_.get_y() * delta.get_y()) /
      (vel_mag * delta_mag);
  cosine = std::max(-1.0, std::min(1.0, cosine));

  return std::acos(cosine);
}

void Boid::update(double delta_t, const std::vector<Boid>& neighbors,
                  double separation_dist, double separation_coeff,
                  double cohesion_coeff, double alignment_coeff) {
  assert(delta_t >= 0);

  point::Point separation_force{0, 0};
  point::Point cohesion_force{0, 0};
  point::Point alignment_force{0, 0};

  if (!neighbors.empty()) {
    for (const auto& neighbor : neighbors) {
      double dist = point::relative_position(neighbor.get_position(), position_)
                        .distance();
      if (dist < separation_dist) {
        separation_force =
            separation_force +
            point::relative_position(neighbor.get_position(), position_);
      }
      cohesion_force = cohesion_force + point::relative_position(
                                            position_, neighbor.get_position());
      alignment_force = alignment_force + neighbor.get_velocity();
    }

    cohesion_force = (cohesion_force / static_cast<double>(neighbors.size())) *
                     cohesion_coeff;
    alignment_force =
        ((alignment_force / static_cast<double>(neighbors.size())) -
         velocity_) *
        alignment_coeff;
    separation_force = separation_force * separation_coeff;
  }

  velocity_ = velocity_ + separation_force + cohesion_force + alignment_force;

  if (velocity_.distance() > constants::max_velocity) {
    velocity_ = velocity_ * (constants::max_velocity / velocity_.distance());
  }

  position_ = position_ + velocity_ * delta_t;

  // Pac-Man wrapping
  if (position_.get_x() < 0)
    position_.set_x(position_.get_x() + constants::window_width);
  else if (position_.get_x() >= constants::window_width)
    position_.set_x(position_.get_x() - constants::window_width);

  if (position_.get_y() < 0)
    position_.set_y(position_.get_y() + constants::window_height);
  else if (position_.get_y() >= constants::window_height)
    position_.set_y(position_.get_y() - constants::window_height);
}

// --- Prey ---

void Prey::update(double delta_t, const std::vector<Boid>& neighbors,
                  double separation_dist, double separation_coeff,
                  double cohesion_coeff, double alignment_coeff) {
  // Per ora lo stesso comportamento dei Boid normali
  Boid::update(delta_t, neighbors, separation_dist, separation_coeff,
               cohesion_coeff, alignment_coeff);
}

// --- Predator ---

void Predator::update(double delta_t, const std::vector<Boid>& neighbors,
                      double separation_dist, double separation_coeff,
                      double cohesion_coeff, double alignment_coeff) {
  // Anche qui, logica identica. In futuro potrai aggiungere
  // comportamento speciale (e.g. inseguire i Prey)
  Boid::update(delta_t, neighbors, separation_dist, separation_coeff,
               cohesion_coeff, alignment_coeff);
}

}  // namespace boid

// #include "../include/boid.hpp"

// #include <cassert>
// #include <cmath>

// #include "../include/constants.hpp"

// namespace boid {
// // Constructor
// Boid::Boid(point::Point position, point::Point velocity)
//     : position_{position}, velocity_{velocity} {}

// // getters
// point::Point Boid::get_position() const { return position_; }
// point::Point Boid::get_velocity() const { return velocity_; }

// // setters
// void Boid::set_position(const point::Point& p) { position_ = p; }
// void Boid::set_velocity(const point::Point& v) { velocity_ = v; }
// // angle

// double Boid::angle(const Boid& other) const {
//   point::Point delta =
//       point::relative_position(this->position_, other.get_position());

//   double vel_mag = velocity_.distance();
//   double delta_mag = delta.distance();

//   if (vel_mag == 0.0 || delta_mag == 0.0) {
//     return 0.0;
//   }

//   double cosine =
//       (velocity_.get_x() * delta.get_x() + velocity_.get_y() * delta.get_y())
//       / (vel_mag * delta_mag);
//   cosine = std::max(-1.0, std::min(1.0, cosine));

//   return std::acos(cosine);
// }

// // flight rules
// point::Point Boid::alignment(const std::vector<Boid>& neighbors,
//                              double alignment_coeff) const {
//   if (neighbors.empty()) return point::Point{0., 0.};

//   point::Point avg_velocity{0., 0.};
//   for (const auto& neighbor : neighbors) {
//     avg_velocity = avg_velocity + neighbor.get_velocity();
//   }
//   avg_velocity = avg_velocity / static_cast<double>(neighbors.size());

//   return (avg_velocity - velocity_) * alignment_coeff;
// }

// point::Point Boid::separation(const std::vector<Boid>& neighbors,
//                               double separation_dist,
//                               double separation_coeff) const {
//   point::Point force{0., 0.};
//   for (const auto& neighbor : neighbors) {
//     double dist =
//         point::relative_position(neighbor.get_position(),
//         position_).distance();
//     if (dist < separation_dist) {
//       force =
//           force + point::relative_position(neighbor.get_position(),
//           position_);
//     }
//   }
//   return force * separation_coeff;
// }

// point::Point Boid::cohesion(const std::vector<Boid>& neighbors,
//                             double cohesion_coeff) const {
//   if (neighbors.empty()) return point::Point{0., 0.};

//   point::Point center_of_mass{0, 0};
//   for (const auto& neighbor : neighbors) {
//     center_of_mass = center_of_mass + point::relative_position(
//                                           position_,
//                                           neighbor.get_position());
//   }
//   center_of_mass = center_of_mass / static_cast<double>(neighbors.size());

//   return center_of_mass * cohesion_coeff;
// }

// void Boid::update(double delta_t, const std::vector<Boid>& neighbors,
//                   double separation_dist, double separation_coeff,
//                   double cohesion_coeff, double alignment_coeff) {
//   assert(delta_t >= 0);

//   point::Point separation_force =
//       separation(neighbors, separation_dist, separation_coeff);
//   point::Point cohesion_force = cohesion(neighbors, cohesion_coeff);
//   point::Point alignment_force = alignment(neighbors, alignment_coeff);

//   velocity_ = velocity_ + separation_force + cohesion_force +
//   alignment_force;

//   if (velocity_.distance() > constants::max_velocity) {
//     velocity_ = velocity_ * (constants::max_velocity / velocity_.distance());
//   }

//   position_ = position_ + velocity_ * delta_t;

//   // Effetto Pac-Man
//   if (position_.get_x() < 0)
//     position_.set_x(position_.get_x() + constants::window_width);
//   else if (position_.get_x() >= constants::window_width)
//     position_.set_x(position_.get_x() - constants::window_width);

//   if (position_.get_y() < 0)
//     position_.set_y(position_.get_y() + constants::window_height);
//   else if (position_.get_y() >= constants::window_height)
//     position_.set_y(position_.get_y() - constants::window_height);
// }

// }  // namespace boid
