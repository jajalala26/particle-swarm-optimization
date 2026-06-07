#pragma once

#include <limits>
#include <vector>

namespace pso {

struct Result {
    std::vector<double> best_pos;
    double best_fitness  = std::numeric_limits<double>::infinity();
    int    iterations    = 0;
    bool   converged     = false;
};

} // namespace pso
