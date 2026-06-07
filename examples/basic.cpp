#include <iostream>
#include "../include/pso/pso.hpp"
#include "../benchmarks/functions.hpp"

// Minimal end-to-end example: optimize Sphere in 5 dimensions.
//
// TODO: once PSO is implemented, this should print something like:
//   Best fitness: 0.000001 at (0.001, -0.001, ...)
//   Converged in 87 iterations

int main() {
    pso::Config cfg;
    cfg.n_dimensions = 5;
    cfg.n_particles  = 20;
    cfg.max_iter     = 500;
    cfg.bounds       = std::vector<pso::Bounds>(5, {-10.0, 10.0});

    pso::PSO optimizer(cfg, benchmarks::Sphere{});
    auto result = optimizer.run();

    std::cout << "Best fitness: " << result.best_fitness << "\n";
    std::cout << "Converged: " << (result.converged ? "yes" : "no") << "\n";
    return 0;
}
