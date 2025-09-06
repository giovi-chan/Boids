
#ifndef STATISTICS_HPP
#define STATISTICS_HPP

namespace statistics {
struct Statistics {
  double mean_distance;
  double dev_distance;
  double mean_velocity;
  double dev_velocity;

  Statistics();

  Statistics(double mean_distance, double dev_distance, double mean_velocity,
             double dev_velocity);
};
}  // namespace statistics

#endif