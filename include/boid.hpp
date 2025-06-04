#ifndef BOID_HPP
#define BOID_HPP

#include <vector>

#include "point.hpp"

namespace boid{
class Boid {
  point::Point position_;
  point::Point velocity_;

 public:
  Boid(point::Point position = point::Point(),
       point::Point velocity = point::Point());

  point::Point get_position() const;
  point::Point get_velocity() const;

  void update(double delta_t, const std::vector<Boid>& neighbors,
              double separation_dist, double separation_coeff,
              double cohesion_coeff, double alignment_coeff);

  point::Point separation(const std::vector<Boid>& neighbors,
                          double separation_dist,
                          double separation_coeff) const;
  point::Point cohesion(const std::vector<Boid>& neighbors,
                        double cohesion_coeff) const;
  point::Point alignment(const std::vector<Boid>& neighbors,
                         double alignment_coeff) const;
};


point::Point relative_position(const point::Point& , const point::Point& );

}
#endif