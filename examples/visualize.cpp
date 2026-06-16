#include <iostream>
#include <memory>
#include <string>
#include "../include/pso/pso.hpp"
#include "../include/pso/observer.hpp"
#include "../benchmarks/functions.hpp"

static void usage(const char *prog) {
    std::fprintf(stderr,
        "Usage: %s [--inertia static|linear|random] [--topology gbest|lbest]\n",
        prog);
}

int main(int argc, char *argv[]) {
    std::string inertia_name  = "linear";
    std::string topology_name = "gbest";

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if ((a == "--inertia" || a == "--topology") && i + 1 < argc) {
            if (a == "--inertia")  inertia_name  = argv[++i];
            else                   topology_name = argv[++i];
        } else {
            usage(argv[0]);
            return 1;
        }
    }

    pso::Config cfg;
    cfg.n_dimensions = 2;
    cfg.n_particles  = 20;
    cfg.max_iter     = 200;
    cfg.seed         = 42;
    cfg.bounds       = std::vector<pso::Bounds>(2, {-10.0, 10.0});

    if      (inertia_name == "static") cfg.inertia = std::make_shared<pso::StaticInertia>();
    else if (inertia_name == "random") cfg.inertia = std::make_shared<pso::RandomInertia>();
    else if (inertia_name == "linear") cfg.inertia = std::make_shared<pso::LinearDecayInertia>();
    else { std::fprintf(stderr, "Unknown inertia: %s\n", inertia_name.c_str()); return 1; }

    if      (topology_name == "gbest") cfg.topology = std::make_shared<pso::GBestTopology>();
    else if (topology_name == "lbest") cfg.topology = std::make_shared<pso::LBestTopology>();
    else { std::fprintf(stderr, "Unknown topology: %s\n", topology_name.c_str()); return 1; }

    std::fprintf(stderr, "inertia=%-6s  topology=%s\n",
                 inertia_name.c_str(), topology_name.c_str());

    pso::PositionObserver obs("positions.csv");
    pso::PSO optimizer(cfg, benchmarks::Sphere{});
    auto result = optimizer.run({&obs});

    std::cout << "best_fitness=" << result.best_fitness << "\n";
    std::cout << "best_pos=(" << result.best_pos[0] << "," << result.best_pos[1] << ")\n";
    std::cout << "converged=" << (result.converged ? "yes" : "no") << "\n";
    return 0;
}
