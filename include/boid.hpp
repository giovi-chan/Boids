#ifndef BOID_HPP
#define BOID_HPP

#include <memory>
#include <vector>

#include "point.hpp"

namespace boid {

class Boid {
 protected:
  point::Point position_;
  point::Point velocity_;

 public:
  Boid() = default;
  Boid(point::Point const &position, point::Point const &velocity);

  virtual ~Boid() = default;

  point::Point get_position() const;
  point::Point get_velocity() const;
  void setBoid(point::Point position, point::Point velocity);
  double angle(const Boid &other) const;

  point::Point separation(double s, double ds,
                          const std::vector<std::shared_ptr<Boid>> &near) const;
};

// ---------------------------

class Prey final : public Boid {
 public:
  Prey();
  Prey(point::Point const &position, point::Point const &velocity);

  point::Point alignment(
      double a, const std::vector<std::shared_ptr<Boid>> &near_prey) const;

  point::Point cohesion(
      double c, const std::vector<std::shared_ptr<Boid>> &near_prey) const;

  point::Point repel(
      double r, const std::vector<std::shared_ptr<Boid>> &near_predators) const;
};

class Predator final : public Boid {
 public:
  Predator();
  Predator(point::Point const &position, point::Point const &velocity);

  point::Point chase(double ch,
                     const std::vector<std::shared_ptr<Boid>> &near_prey) const;
};
}  // namespace boid
#endif