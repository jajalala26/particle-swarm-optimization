#pragma once

#include <span>
#include <string>
#include <vector>
#include "particle.hpp"

namespace pso {

struct Result; // forward declaration — defined in pso.hpp

struct IObserver {
    virtual ~IObserver() = default;
    virtual void on_iteration(int iter, double best_fitness,
                              std::span<const Particle> particles) = 0;
    virtual void on_complete(const Result& result) = 0;
};

struct ConsoleObserver : IObserver {
    void on_iteration(int iter, double best_fitness,
                      std::span<const Particle> particles) override;
    void on_complete(const Result& result) override;
    // TODO: print iter, best_fitness, delta from previous best
};

struct CsvObserver : IObserver {
    explicit CsvObserver(std::string path);
    void on_iteration(int iter, double best_fitness,
                      std::span<const Particle> particles) override;
    void on_complete(const Result& result) override;
    // TODO: write "iter,best_fitness,mean_fitness\n" rows to path_
private:
    std::string path_;
};

struct CompositeObserver : IObserver {
    void add(IObserver* obs);
    void on_iteration(int iter, double best_fitness,
                      std::span<const Particle> particles) override;
    void on_complete(const Result& result) override;
private:
    std::vector<IObserver*> observers_;
};

// --- Inline stub implementations (fill these in) ---

inline void ConsoleObserver::on_iteration(int /*iter*/, double /*best_fitness*/,
                                          std::span<const Particle>) {
    // TODO: implement
}
inline void ConsoleObserver::on_complete(const Result&) {
    // TODO: implement
}

inline CsvObserver::CsvObserver(std::string path) : path_(std::move(path)) {}
inline void CsvObserver::on_iteration(int /*iter*/, double /*best_fitness*/,
                                      std::span<const Particle>) {
    // TODO: implement
}
inline void CsvObserver::on_complete(const Result&) {
    // TODO: implement
}

inline void CompositeObserver::add(IObserver* obs) { observers_.push_back(obs); }
inline void CompositeObserver::on_iteration(int iter, double best_fitness,
                                            std::span<const Particle> particles) {
    for (auto* o : observers_) o->on_iteration(iter, best_fitness, particles);
}
inline void CompositeObserver::on_complete(const Result& result) {
    for (auto* o : observers_) o->on_complete(result);
}

} // namespace pso
