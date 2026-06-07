#pragma once

#include <span>

namespace benchmarks {

// All functions: minimize over R^n. Known optimum is 0.0 at origin unless noted.

struct Sphere {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
    // TODO: sum of squares: sum(x_i^2)
};

struct Rastrigin {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
    // TODO: 10*n + sum(x_i^2 - 10*cos(2*pi*x_i))
    // Typical bounds: [-5.12, 5.12] per dimension
};

struct Rosenbrock {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
    // TODO: sum(100*(x_{i+1} - x_i^2)^2 + (1 - x_i)^2)
    // Optimum at (1, 1, ..., 1)
};

struct Ackley {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
    // TODO: -20*exp(-0.2*sqrt(mean(x_i^2))) - exp(mean(cos(2*pi*x_i))) + 20 + e
    // Typical bounds: [-32.768, 32.768] per dimension
};

// --- Inline stub implementations (fill these in) ---

inline double Sphere::operator()(std::span<const double> /*x*/) const {
    return 0.0; // TODO: implement
}

inline double Rastrigin::operator()(std::span<const double> /*x*/) const {
    return 0.0; // TODO: implement
}

inline double Rosenbrock::operator()(std::span<const double> /*x*/) const {
    return 0.0; // TODO: implement
}

inline double Ackley::operator()(std::span<const double> /*x*/) const {
    return 0.0; // TODO: implement
}

} // namespace benchmarks
