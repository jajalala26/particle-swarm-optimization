#pragma once

#include <concepts>
#include <span>
#include <vector>
#include "config.hpp"
#include "observer.hpp"
#include "particle.hpp"
#include "result.hpp"
#include <barrier>
#include <random>
#include <thread>

namespace pso
{

  template <typename CostFn>
  concept CostFunction = requires(CostFn fn, std::span<const double> pos) {
    { fn(pos) } -> std::convertible_to<double>;
  };

  template <CostFunction CostFn>
  class PSO
  {
  public:
    PSO(Config cfg, CostFn fn);

    // Run the optimization. Observers are notified each iteration and on completion.
    Result run(const std::vector<IObserver *> &observers = {});

  private:
    Config cfg_;
    CostFn fn_;
    std::vector<Particle> particles_;

    void init_particles();
    // TODO: implement iteration loop with std::thread + std::barrier
    // if cfg_.n_threads == 0, use std::thread::hardware_concurrency() (default to 1 if it returns 0)
    // Per iteration:
    //   1. Evaluate cost for each particle (parallel)
    //   2. Update personal bests
    //   3. Call cfg_.topology->get_attractor() per particle
    //   4. Update velocities and positions (parallel)
    //   5. Get inertia weight from cfg_.inertia->get_w()
    //   6. Check convergence against cfg_.tol
    //   7. Notify observers: observer->on_iteration(iter, best_fitness, particles)
    //   8. After loop exits, call observer->on_complete(result) for each observer
    // Suggested includes when implementing: <thread>, <barrier>, <random>
  };

  // --- Inline stub implementations (fill these in) ---

  template <CostFunction CostFn>
  PSO<CostFn>::PSO(Config cfg, CostFn fn)
      : cfg_(std::move(cfg)), fn_(std::move(fn)) {}

  template <CostFunction CostFn>
  Result PSO<CostFn>::run(const std::vector<IObserver *> & /*observers*/)
  {
    init_particles();
    // TODO: implement main optimization loop

    return Result{};
  }

  template <CostFunction CostFn>
  void PSO<CostFn>::init_particles()
  {
    std::mt19937 rng(cfg_.seed != 0 ? cfg_.seed : std::random_device{}());

    // init particles_ vector with n_particles Particle obj
    particles_.resize(cfg_.n_particles);

    for (auto &p : particles_)
    {
      p.position.resize(cfg_.n_dimensions);
      p.velocity.assign(cfg_.n_dimensions, 0.0);
      p.pbest_fitness = std::numeric_limits<double>::infinity();

      // for each dimension in the cur particle
      for (int d = 0; d < cfg_.n_dimensions; ++d)
      {
        std::uniform_real_distribution<double> dist(
            cfg_.bounds[d].lo, cfg_.bounds[d].hi);
        p.position[d] = dist(rng);
      }
      p.pbest_pos = p.position;
    }
  }

} // namespace pso
