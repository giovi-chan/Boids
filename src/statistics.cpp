#include "../include/statistics.hpp"

namespace statistics {

Statistics::Statistics()
    : mean_distance{0.},
      dev_distance{0.},
      mean_velocity{0.},
      dev_velocity{0.} {}
Statistics::Statistics(const double mean_distance, const double dev_distance,
                       const double mean_velocity, const double dev_velocity)
    : mean_distance{mean_distance},
      dev_distance{dev_distance},
      mean_velocity{mean_velocity},
      dev_velocity{dev_velocity} {}
}  // namespace statistics