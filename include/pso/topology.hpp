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
        std::span<const Particle> particles) = 0;
};

struct GBestTopology : ITopology {
    const std::vector<double>& get_attractor(
        int particle_idx,
        std::span<const Particle> particles) override;
    // TODO: track global best across the swarm
};

struct LBestTopology : ITopology {
    explicit LBestTopology(int neighborhood_size = 3);
    const std::vector<double>& get_attractor(
        int particle_idx,
        std::span<const Particle> particles) override;
    // TODO: find best in ring neighborhood of size neighborhood_size_
private:
    int neighborhood_size_;
};

// --- Inline stub implementations (fill these in) ---

inline const std::vector<double>& GBestTopology::get_attractor(
    int /*particle_idx*/,
    std::span<const Particle> particles)
{
    // TODO: return position of particle with lowest pbest_fitness
    return particles[0].pbest_pos;
}

inline LBestTopology::LBestTopology(int neighborhood_size)
    : neighborhood_size_(neighborhood_size) {}

inline const std::vector<double>& LBestTopology::get_attractor(
    int /*particle_idx*/,
    std::span<const Particle> particles)
{
    // TODO: return best pbest_pos in ring neighborhood
    return particles[0].pbest_pos;
}

} // namespace pso
