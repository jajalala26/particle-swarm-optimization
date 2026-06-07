#pragma once

#include <concepts>
#include <span>
#include <vector>
#include "config.hpp"
#include "observer.hpp"
#include "particle.hpp"

namespace pso {

struct Result {
    std::vector<double> best_pos;
    double best_fitness  = 0.0;
    int    iterations    = 0;
    bool   converged     = false;
};

template<typename CostFn>
concept CostFunction = requires(CostFn fn, std::span<const double> pos) {
    { fn(pos) } -> std::convertible_to<double>;
};

template<CostFunction CostFn>
class PSO {
public:
    PSO(Config cfg, CostFn fn);

    // Run the optimization. Observers are notified each iteration and on completion.
    Result run(std::vector<IObserver*> observers = {});

private:
    Config cfg_;
    CostFn fn_;
    std::vector<Particle> particles_;

    void init_particles();
    // TODO: implement iteration loop with std::thread + std::barrier
    // Per iteration:
    //   1. Evaluate cost for each particle (parallel)
    //   2. Update personal bests
    //   3. Call cfg_.topology->get_attractor() per particle
    //   4. Update velocities and positions (parallel)
    //   5. Get inertia weight from cfg_.inertia->get_w()
    //   6. Check convergence against cfg_.tol
    //   7. Notify observers
};

// --- Inline stub implementations (fill these in) ---

template<CostFunction CostFn>
PSO<CostFn>::PSO(Config cfg, CostFn fn)
    : cfg_(std::move(cfg)), fn_(std::move(fn)) {}

template<CostFunction CostFn>
Result PSO<CostFn>::run(std::vector<IObserver*> /*observers*/) {
    init_particles();
    // TODO: implement main optimization loop
    return Result{};
}

template<CostFunction CostFn>
void PSO<CostFn>::init_particles() {
    // TODO: initialize particles with random positions in cfg_.bounds
    //       and zero (or small random) velocities
}

} // namespace pso
