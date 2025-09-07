#include "../include/statistics.hpp"

namespace statistics {

Statistics::Statistics()
    : mean_distance{0.},
      dev_distance{0.},
      mean_velocity{0.},
      dev_velocity{0.} {}

Statistics::Statistics(const double mean_dist, const double dev_dist,
                       const double mean_vel, const double dev_vel)
    : mean_distance{mean_dist},
      dev_distance{dev_dist},
      mean_velocity{mean_vel},
      dev_velocity{dev_vel} {}
}  // namespace statistics
