#include <gtest/gtest.h>
#include "../include/pso/pso.hpp"
#include "../benchmarks/functions.hpp"

// ------------------------------------------------------------
// Inertia strategy tests
// ------------------------------------------------------------

TEST(StaticInertia, ReturnsConstantW) {
    pso::StaticInertia inertia(0.7);
    EXPECT_DOUBLE_EQ(inertia.get_w(0, 100), 0.7);
    EXPECT_DOUBLE_EQ(inertia.get_w(50, 100), 0.7);
    EXPECT_DOUBLE_EQ(inertia.get_w(99, 100), 0.7);
}

TEST(LinearDecayInertia, StartsAtWStartEndsAtWEnd) {
    pso::LinearDecayInertia inertia(0.9, 0.4);
    // TODO: implement — first iter should be ~0.9, last should be ~0.4
    EXPECT_NEAR(inertia.get_w(0, 100), 0.9, 1e-6);
    EXPECT_NEAR(inertia.get_w(99, 100), 0.4, 1e-6);
}

TEST(RandomInertia, ReturnsValueInRange) {
    pso::RandomInertia inertia(0.4, 0.9);
    for (int i = 0; i < 100; ++i) {
        double w = inertia.get_w(i, 100);
        EXPECT_GE(w, 0.4);
        EXPECT_LE(w, 0.9);
    }
}

// ------------------------------------------------------------
// PSO integration tests (these will pass once you implement run())
// ------------------------------------------------------------

TEST(PSO, SphereConvergesInLowDimensions) {
    pso::Config cfg;
    cfg.n_dimensions = 2;
    cfg.n_particles  = 20;
    cfg.max_iter     = 500;
    cfg.seed         = 42;
    cfg.bounds       = std::vector<pso::Bounds>(2, {-10.0, 10.0});

    pso::PSO optimizer(cfg, benchmarks::Sphere{});
    auto result = optimizer.run();

    EXPECT_LT(result.best_fitness, 1e-4);
}

TEST(PSO, DeterministicWithFixedSeed) {
    pso::Config cfg;
    cfg.n_dimensions = 3;
    cfg.n_particles  = 15;
    cfg.max_iter     = 100;
    cfg.seed         = 99;
    cfg.bounds       = std::vector<pso::Bounds>(3, {-5.0, 5.0});

    pso::PSO opt1(cfg, benchmarks::Sphere{});
    pso::PSO opt2(cfg, benchmarks::Sphere{});

    auto r1 = opt1.run();
    auto r2 = opt2.run();

    EXPECT_DOUBLE_EQ(r1.best_fitness, r2.best_fitness);
}

TEST(PSO, GBestAndLBestBothRun) {
    pso::Config cfg;
    cfg.n_dimensions = 2;
    cfg.n_particles  = 10;
    cfg.max_iter     = 50;
    cfg.bounds       = std::vector<pso::Bounds>(2, {-5.0, 5.0});

    cfg.topology = std::make_shared<pso::GBestTopology>();
    pso::PSO gbest_opt(cfg, benchmarks::Sphere{});
    EXPECT_NO_THROW(gbest_opt.run());

    cfg.topology = std::make_shared<pso::LBestTopology>(3);
    pso::PSO lbest_opt(cfg, benchmarks::Sphere{});
    EXPECT_NO_THROW(lbest_opt.run());
}
