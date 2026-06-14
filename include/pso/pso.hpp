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
  };

  // --- Inline stub implementations (fill these in) ---

  template <CostFunction CostFn>
  PSO<CostFn>::PSO(Config cfg, CostFn fn)
      : cfg_(std::move(cfg)), fn_(std::move(fn)) {}

  template <CostFunction CostFn>
  Result PSO<CostFn>::run(const std::vector<IObserver *> &observers)
  {
    init_particles();

    // figure out how many threads to use
    unsigned int n_threads = cfg_.n_threads > 0
                                 ? static_cast<unsigned int>(cfg_.n_threads)
                                 : std::max(1u, std::thread::hardware_concurrency());

    Result result;
    double prev_best = std::numeric_limits<double>::infinity();

    // one cached attractor position per particle (filled by thread 0)
    std::vector<std::vector<double>> attractors(cfg_.n_particles);

    // one RNG per thread so threads don't share (and race on) one RNG
    std::vector<std::mt19937> rngs(n_threads);
    {
      std::mt19937 seed_gen(cfg_.seed != 0 ? cfg_.seed : std::random_device{}());
      for (auto &r : rngs)
        r.seed(seed_gen());
    }

    // the barrier — all n_threads must arrive before any continue
    std::barrier sync(n_threads);

    // the worker lambda — each thread runs this with its own tid
    // [&] captures everything in the enclosing scope by reference
    auto worker = [&](unsigned int tid)
    {
      int lo = (cfg_.n_particles * tid) / n_threads;
      int hi = (cfg_.n_particles * (tid + 1)) / n_threads;
      std::uniform_real_distribution<double> unit(0.0, 1.0);

      for (int iter = 0; iter < cfg_.max_iter; ++iter)
      {

        // --- 1. evaluate cost, update personal best ---
        for (int i = lo; i < hi; ++i)
        {
          double f = fn_(particles_[i].position);
          if (f < particles_[i].pbest_fitness)
          {
            particles_[i].pbest_fitness = f;
            particles_[i].pbest_pos = particles_[i].position;
          }
        }

        sync.arrive_and_wait(); // barrier 1: all pbests are now fresh

        // --- 2. thread 0 does the single-threaded work ---
        if (tid == 0)
        {
          // find global best
          double cur_best = std::numeric_limits<double>::infinity();
          for (const auto &p : particles_)
          {
            if (p.pbest_fitness < cur_best)
            {
              cur_best = p.pbest_fitness;
              result.best_pos = p.pbest_pos;
            }
          }
          result.best_fitness = cur_best;
          result.iterations = iter + 1;
          result.converged = std::abs(cur_best - prev_best) < cfg_.tol;
          prev_best = cur_best;

          // cache attractor for every particle
          for (int i = 0; i < cfg_.n_particles; ++i)
            attractors[i] = cfg_.topology->get_attractor(i, particles_);

          for (auto *obs : observers)
            obs->on_iteration(iter, cur_best, particles_);
        }

        sync.arrive_and_wait(); // barrier 2: attractors and converged flag are ready

        if (result.converged)
          break;

        // --- 3. update velocity and position ---
        double w = cfg_.inertia->get_w(iter, cfg_.max_iter);
        for (int i = lo; i < hi; ++i)
        {
          for (int d = 0; d < cfg_.n_dimensions; ++d)
          {
            double r1 = unit(rngs[tid]);
            double r2 = unit(rngs[tid]);

            // velocity update equation
            particles_[i].velocity[d] =
                w * particles_[i].velocity[d] + cfg_.c1 * r1 * (particles_[i].pbest_pos[d] - particles_[i].position[d]) + cfg_.c2 * r2 * (attractors[i][d] - particles_[i].position[d]);

            particles_[i].position[d] += particles_[i].velocity[d];
          }
        }

        sync.arrive_and_wait(); // barrier 3: all positions updated before next iter
      }
    };

    // launch n_threads threads, each running worker with its own tid
    std::vector<std::thread> threads;
    threads.reserve(n_threads);
    for (unsigned int t = 0; t < n_threads; ++t)
      threads.emplace_back(worker, t);

    // wait for all threads to finish
    for (auto &th : threads)
      th.join();

    // notify observers that the run is complete
    for (auto *obs : observers)
      obs->on_complete(result);

    return result;
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
