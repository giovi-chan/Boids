#ifndef BOID_HPP
#define BOID_HPP

#include "point.hpp"
#include <vector>

class Boid {
  Point position_;
  Point velocity_;

 public:
  Boid(Point position = Point{0, 0}, Point velocity = Point{0, 0});

  Point position() const;
  Point velocity() const;

  void update(double delta_t, const std::vector<Boid>& neighbors,
              double separation_dist, double separation_coeff,
              double cohesion_coeff, double alignment_coeff);

  Point separation(const std::vector<Boid>& neighbors, double separation_dist,
                   double separation_coeff) const;
  Point cohesion(const std::vector<Boid>& neighbors,
                 double cohesion_coeff) const;
  Point alignment(const std::vector<Boid>& neighbors,
                  double alignment_coeff) const;
};

#endif