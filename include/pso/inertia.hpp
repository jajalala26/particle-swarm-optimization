#pragma once
#include <random>

namespace pso
{

  struct IInertia
  {
    virtual ~IInertia() = default;
    virtual double get_w(int current_iter, int max_iter) const = 0;
  };

  struct StaticInertia : IInertia
  {
    explicit StaticInertia(double w = 0.7);
    double get_w(int current_iter, int max_iter) const override;

  private:
    double w_;
  };

  struct LinearDecayInertia : IInertia
  {
    LinearDecayInertia(double w_start = 0.9, double w_end = 0.4);
    double get_w(int current_iter, int max_iter) const override;

  private:
    double w_start_;
    double w_end_;
  };

  struct RandomInertia : IInertia
  {
    RandomInertia(double w_min = 0.4, double w_max = 0.9);
    double get_w(int current_iter, int max_iter) const override;

  private:
    double w_min_;
    double w_max_;
  };

  // --- Inline stub implementations (fill these in) ---

  inline StaticInertia::StaticInertia(double w) : w_(w) {}
  inline double StaticInertia::get_w(int, int) const { return w_; }

  inline LinearDecayInertia::LinearDecayInertia(double w_start, double w_end)
      : w_start_(w_start), w_end_(w_end) {}
  inline double LinearDecayInertia::get_w(int current_iter, int max_iter) const
  {
    if (max_iter <= 1)
      return w_start_;
    return w_start_ - (w_start_ - w_end_) * (static_cast<double>(current_iter) / (max_iter - 1));
  }

  inline RandomInertia::RandomInertia(double w_min, double w_max)
      : w_min_(w_min), w_max_(w_max) {}
  inline double RandomInertia::get_w(int /*current_iter*/, int /*max_iter*/) const
  {
    // thread local creates new instances per thread
    // rng with random seed
    thread_local std::mt19937 rng{std::random_device{}()};

    // creates dist between min, max inertia, rng supplied for val
    std::uniform_real_distribution<double> dist(w_min_, w_max_);
    return dist(rng);
  }

} // namespace pso
