#ifndef BOID_HPP
#define BOID_HPP

#include <memory>  // for std::shared_ptr
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

  point::Point getPosition() const;
  point::Point getVelocity() const;
  void setBoid(point::Point const &position, point::Point const &velocity);
  double angle(Boid const &other) const;

  point::Point separation(double s, double ds,
                          const std::vector<std::shared_ptr<Boid>> &near) const;

  virtual void clamp(const double min_speed, const double max_speed,
                     point::Point &velocity) = 0;
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

  point::Point repulsion(
      double r, const std::vector<std::shared_ptr<Boid>> &near_predators) const;

  void clamp(const double min_speed, const double max_speed,
             point::Point &velocity) override;
};

class Predator final : public Boid {
 public:
  Predator();
  Predator(point::Point const &position, point::Point const &velocity);

  point::Point chase(double ch,
                     const std::vector<std::shared_ptr<Boid>> &near_prey) const;

  void clamp(const double min_speed, const double max_speed,
             point::Point &velocity) override;
};
}  // namespace boid
#endif