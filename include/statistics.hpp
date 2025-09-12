
#ifndef STATISTICS_HPP
#define STATISTICS_HPP

namespace statistics {
struct Statistics {
  double mean_distance;
  double dev_distance;
  double mean_velocity;
  double dev_velocity;

  Statistics();

  Statistics(double mean_dist, double dev_dist, double mean_vel,
             double dev_vel);
};
}  // namespace statistics

#endif
