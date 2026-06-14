#pragma once

#include <span>
#include <cmath>
#include <numbers>

namespace benchmarks
{

  // All functions: minimize over R^n. known_optimum is the minimum function value.
  // Note: the minimizing *position* is not always the origin (see Rosenbrock).

  struct Sphere
  {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
  };

  struct Rastrigin
  {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
  };

  struct Rosenbrock
  {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
  };

  struct Ackley
  {
    static constexpr double known_optimum = 0.0;
    double operator()(std::span<const double> x) const;
  };

  // --- Inline stub implementations ---

  inline double Sphere::operator()(std::span<const double> x) const
  {
    double sum = 0.0;
    for (double xi : x)
      sum += xi * xi;
    return sum;
  }

  inline double Rastrigin::operator()(std::span<const double> x) const
  {
    constexpr double pi = std::numbers::pi;
    double sum = 10.0 * static_cast<double>(x.size());
    for (double xi : x)
      sum += xi * xi - 10.0 * std::cos(2.0 * pi * xi);
    return sum;
  }

  inline double Rosenbrock::operator()(std::span<const double> x) const
  {
    double sum = 0.0;
    for (std::size_t i = 0; i + 1 < x.size(); ++i)
    {
      double t1 = x[i + 1] - x[i] * x[i];
      double t2 = 1.0 - x[i];
      sum += 100.0 * t1 * t1 + t2 * t2;
    }
    return sum;
  }

  inline double Ackley::operator()(std::span<const double> x) const
  {
    constexpr double pi = std::numbers::pi;
    int n = static_cast<int>(x.size());
    double sum_sq = 0.0, sum_cos = 0.0;
    for (double xi : x)
    {
      sum_sq += xi * xi;
      sum_cos += std::cos(2.0 * pi * xi);
    }
    return -20.0 * std::exp(-0.2 * std::sqrt(sum_sq / n)) - std::exp(sum_cos / n) + 20.0 + std::numbers::e;
  }

} // namespace benchmarks
