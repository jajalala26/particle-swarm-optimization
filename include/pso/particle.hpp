#pragma once

#include <limits>
#include <vector>

namespace pso {

struct Particle {
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> pbest_pos;
    double              pbest_fitness = std::numeric_limits<double>::infinity();
};

} // namespace pso
