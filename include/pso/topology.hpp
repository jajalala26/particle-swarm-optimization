#pragma once

#include <span>
#include <vector>
#include "particle.hpp"

namespace pso {

struct ITopology {
    virtual ~ITopology() = default;
    // Returns the position this particle should be attracted toward.
    virtual const std::vector<double>& get_attractor(
        int particle_idx,
        std::span<const Particle> particles) const = 0;
};

struct GBestTopology : ITopology {
    const std::vector<double>& get_attractor(
        int particle_idx,
        std::span<const Particle> particles) const override;
    // TODO: store gbest_pos_ as a mutable member; update it here; return ref to member.
    //       Do NOT return a ref into `particles` — the span lifetime is not guaranteed.
};

struct LBestTopology : ITopology {
    explicit LBestTopology(int neighborhood_size = 3);
    const std::vector<double>& get_attractor(
        int particle_idx,
        std::span<const Particle> particles) const override;
    // TODO: store lbest_pos_ as a mutable member per particle or as a cached vector.
    //       Return ref to stored member, not into `particles`.
private:
    int neighborhood_size_;
};

// --- Inline stub implementations (fill these in) ---

inline const std::vector<double>& GBestTopology::get_attractor(
    int /*particle_idx*/,
    std::span<const Particle> particles) const
{
    // TODO: return position of particle with lowest pbest_fitness
    return particles[0].pbest_pos;
}

inline LBestTopology::LBestTopology(int neighborhood_size)
    : neighborhood_size_(neighborhood_size) {}

inline const std::vector<double>& LBestTopology::get_attractor(
    int /*particle_idx*/,
    std::span<const Particle> particles) const
{
    // TODO: return best pbest_pos in ring neighborhood
    return particles[0].pbest_pos;
}

} // namespace pso
