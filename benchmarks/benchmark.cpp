#include <iostream>
#include "../include/pso/pso.hpp"
#include "functions.hpp"

// Runs each topology x inertia combination on each benchmark function
// and prints a comparison table.
//
// TODO: implement sweep — for each combination of:
//   - topology: GBestTopology, LBestTopology(3)
//   - inertia:  StaticInertia(0.7), LinearDecayInertia(), RandomInertia()
//   - function: Sphere, Rastrigin, Rosenbrock, Ackley
// Run PSO and record best_fitness + iterations, then print as a table.

int main() {
    std::cout << "PSO Benchmark\n";
    std::cout << "TODO: implement sweep\n";
    return 0;
}
