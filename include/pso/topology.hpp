#pragma once

#include <span>
#include <vector>
#include "particle.hpp"

namespace pso
{

  struct ITopology
  {
    virtual ~ITopology() = default;
    // Returns the position this particle should be attracted toward.
    virtual const std::vector<double> &get_attractor(
        int particle_idx,
        std::span<const Particle> particles) const = 0;
  };

  struct GBestTopology : ITopology
  {
    mutable std::vector<double> gbest_pos_;
    const std::vector<double> &get_attractor(
        int particle_idx,
        std::span<const Particle> particles) const override;
  };

  struct LBestTopology : ITopology
  {
    mutable std::vector<double> lbest_pos_;
    explicit LBestTopology(int neighborhood_size = 3);
    const std::vector<double> &get_attractor(
        int particle_idx,
        std::span<const Particle> particles) const override;

  private:
    int neighborhood_size_;
  };

  // --- Inline stub implementations ---

  inline const std::vector<double> &GBestTopology::get_attractor(
      int /*particle_idx*/,
      std::span<const Particle> particles) const
  {
    const Particle *best = &particles[0];
    for (const auto &p : particles)
      if (p.pbest_fitness < best->pbest_fitness)
        best = &p;

    gbest_pos_ = best->pbest_pos;
    return gbest_pos_;
  }

  inline LBestTopology::LBestTopology(int neighborhood_size)
      : neighborhood_size_(neighborhood_size) {}

  inline const std::vector<double> &LBestTopology::get_attractor(
      int particle_idx,
      std::span<const Particle> particles) const
  {
    int N = static_cast<int>(particles.size());
    int half = neighborhood_size_ / 2;
    const Particle *best = nullptr;

    for (int j = 0; j < neighborhood_size_; ++j)
    {
      int idx = ((particle_idx - half + j) % N + N) % N;
      if (!best || particles[idx].pbest_fitness < best->pbest_fitness)
        best = &particles[idx];
    }

    lbest_pos_ = best->pbest_pos;
    return lbest_pos_;
  }

} // namespace pso
