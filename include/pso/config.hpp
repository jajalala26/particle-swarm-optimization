#pragma once

#include <memory>
#include <vector>
#include "inertia.hpp"
#include "topology.hpp"

namespace pso {

struct Bounds {
    double lo;
    double hi;
};

struct Config {
    int n_particles  = 30;
    int n_dimensions = 2;
    int max_iter     = 500;
    int n_threads    = 0;      // 0 = std::thread::hardware_concurrency()
    unsigned int seed = 0;     // 0 = non-deterministic

    double tol = 1e-6;         // convergence threshold on gbest delta
    double c1  = 1.496;        // cognitive coefficient
    double c2  = 1.496;        // social coefficient

    std::vector<Bounds> bounds;

    std::shared_ptr<IInertia>  inertia  = std::make_shared<LinearDecayInertia>();
    std::shared_ptr<ITopology> topology = std::make_shared<GBestTopology>();
};

} // namespace pso
