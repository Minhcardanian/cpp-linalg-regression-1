#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Matrix.hpp"
#include "Vector.hpp"
#include "LinearSystem.hpp"

void print_usage() {
    std::cout << "Usage: RegressionDemo --data <path> --train-split <0-1> --seed <int>\n";
}

int main(int argc, char* argv[]) {
    std::string data_file;
    double split = 0.8;
    int seed = 42;

    // Simple arg parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--data" && i+1 < argc) data_file = argv[++i];
        else if (arg == "--train-split" && i+1 < argc) split = std::stod(argv[++i]);
        else if (arg == "--seed" && i+1 < argc) seed = std::stoi(argv[++i]);
        else { print_usage(); return 1; }
    }
    if (data_file.empty()) { print_usage(); return 1; }

    // TODO: load CSV into Matrix X and Vector y
    // TODO: split train/test
    // TODO: build normal equations A = X^T X, b = X^T y
    // TODO: solve via PosSymLinSystem or pseudo-inverse
    // TODO: compute and print RMSE

    std::cout << "RegressionDemo: not yet implemented\n";
    return 0;
}
