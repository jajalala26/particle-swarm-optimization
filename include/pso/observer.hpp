#pragma once

#include <span>
#include <string>
#include <vector>
#include "particle.hpp"
#include "result.hpp"

#include <cstdio>
#include <limits>

namespace pso
{

  struct IObserver
  {
    virtual ~IObserver() = default;
    virtual void on_iteration(int iter, double best_fitness,
                              std::span<const Particle> particles) = 0;
    virtual void on_complete(const Result &result) = 0;
  };

  struct ConsoleObserver : IObserver
  {
    void on_iteration(int iter, double best_fitness,
                      std::span<const Particle> particles) override;
    void on_complete(const Result &result) override;
  };

  struct CsvObserver : IObserver
  {
    explicit CsvObserver(std::string path);
    ~CsvObserver();
    void on_iteration(int iter, double best_fitness,
                      std::span<const Particle> particles) override;
    void on_complete(const Result &result) override;

  private:
    std::string path_;
    FILE *file_ = nullptr;
  };

  struct CompositeObserver : IObserver
  {
    void add(IObserver *obs);
    void on_iteration(int iter, double best_fitness,
                      std::span<const Particle> particles) override;
    void on_complete(const Result &result) override;

  private:
    std::vector<IObserver *> observers_;
  };

  // --- Inline stub implementations (fill these in) ---

  inline void ConsoleObserver::on_iteration(int iter, double best_fitness,
                                            std::span<const Particle> /*particles*/)
  {
    static double prev = std::numeric_limits<double>::infinity();
    double delta = best_fitness - prev;
    prev = best_fitness;
    std::printf("[iter %4d]  best = %.6e  delta = %+.3e\n", iter, best_fitness, delta);
  }

  inline void ConsoleObserver::on_complete(const Result &r)
  {
    std::printf("Done. iters=%d  best=%.6e  converged=%s\n",
                r.iterations, r.best_fitness, r.converged ? "yes" : "no");
  }

  inline CsvObserver::CsvObserver(std::string path) : path_(std::move(path))
  {
    file_ = std::fopen(path_.c_str(), "w");
    if (file_)
      std::fprintf(file_, "iter,best_fitness,mean_fitness\n");
  }

  inline CsvObserver::~CsvObserver()
  {
    if (file_)
      std::fclose(file_);
  }

  inline void CsvObserver::on_iteration(int iter, double best_fitness,
                                        std::span<const Particle> particles)
  {
    if (!file_)
      return;
    double sum = 0.0;
    for (const auto &p : particles)
      sum += p.pbest_fitness;
    double mean = particles.empty() ? 0.0 : sum / static_cast<double>(particles.size());
    std::fprintf(file_, "%d,%.6e,%.6e\n", iter, best_fitness, mean);
  }

  inline void CsvObserver::on_complete(const Result &)
  {
    if (file_)
      std::fflush(file_);
  }

  struct PositionObserver : IObserver
  {
    explicit PositionObserver(std::string path);
    ~PositionObserver();
    void on_iteration(int iter, double best_fitness,
                      std::span<const Particle> particles) override;
    void on_complete(const Result &result) override;

  private:
    std::string path_;
    FILE *file_ = nullptr;
  };

  inline PositionObserver::PositionObserver(std::string path) : path_(std::move(path))
  {
    file_ = std::fopen(path_.c_str(), "w");
    if (file_)
      std::fprintf(file_, "iter,particle_idx,x,y,fitness\n");
  }

  inline PositionObserver::~PositionObserver()
  {
    if (file_)
      std::fclose(file_);
  }

  inline void PositionObserver::on_iteration(int iter, double /*best_fitness*/,
                                             std::span<const Particle> particles)
  {
    if (!file_)
      return;
    for (int i = 0; i < static_cast<int>(particles.size()); ++i)
    {
      const auto &p = particles[i];
      std::fprintf(file_, "%d,%d,%.6e,%.6e,%.6e\n",
                   iter, i,
                   p.position[0], p.position[1],
                   p.pbest_fitness);
    }
  }

  inline void PositionObserver::on_complete(const Result &result)
  {
    if (!file_)
      return;
    std::fprintf(file_, "DONE,%.6e,%.6e,%.6e\n",
                 result.best_pos[0], result.best_pos[1],
                 result.best_fitness);
    std::fflush(file_);
  }

  inline void CompositeObserver::add(IObserver *obs) { observers_.push_back(obs); }
  inline void CompositeObserver::on_iteration(int iter, double best_fitness,
                                              std::span<const Particle> particles)
  {
    for (auto *o : observers_)
      o->on_iteration(iter, best_fitness, particles);
  }
  inline void CompositeObserver::on_complete(const Result &result)
  {
    for (auto *o : observers_)
      o->on_complete(result);
  }

} // namespace pso
