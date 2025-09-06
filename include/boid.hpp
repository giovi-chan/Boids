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
  Boid(const point::Point& position, const point::Point& velocity);
  virtual ~Boid() = default;

  point::Point getPosition() const;
  point::Point getVelocity() const;
  void setBoid(const point::Point& position, const point::Point& velocity);
  double angle(const Boid& other) const;

  point::Point separation(const double s, const double ds,
                          const std::vector<std::shared_ptr<Boid>>& near) const;

  virtual void clamp(const double min_speed, const double max_speed,
                     point::Point& velocity) = 0;
};

// ---------------------------

class Prey final : public Boid {
 public:
  Prey();
  Prey(const point::Point& position, const point::Point& velocity);

  point::Point alignment(
      const double a,
      const std::vector<std::shared_ptr<Boid>>& near_prey) const;

  point::Point cohesion(
      const double c,
      const std::vector<std::shared_ptr<Boid>>& near_prey) const;

  point::Point repulsion(
      const double r,
      const std::vector<std::shared_ptr<Boid>>& near_predators) const;

  void clamp(const double min_speed, const double max_speed,
             point::Point& velocity) override;
};

class Predator final : public Boid {
 public:
  Predator();
  Predator(const point::Point& position, const point::Point& velocity);

  point::Point chase(const double ch,
                     const std::vector<std::shared_ptr<Boid>>& near_prey) const;

  void clamp(const double min_speed, const double max_speed,
             point::Point& velocity) override;
};

}  // namespace boid

#endif
