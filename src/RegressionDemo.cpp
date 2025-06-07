// src/RegressionDemo.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>        // for std::array<double,6>
#include <random>
#include <algorithm>    // for std::shuffle
#include <iomanip>
#include <cmath>
#include "Matrix.hpp"
#include "Vector.hpp"
#include "LinearSystem.hpp"

static void print_usage() {
    std::cout << "Usage: RegressionDemo --data <path> --train-split <0-1> --seed <int>\n";
}

int main(int argc, char* argv[]) {
    std::string data_file;
    double train_split = 0.8;
    unsigned seed = 42;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--data" && i+1 < argc) {
            data_file = argv[++i];
        }
        else if (arg == "--train-split" && i+1 < argc) {
            train_split = std::stod(argv[++i]);
        }
        else if (arg == "--seed" && i+1 < argc) {
            seed = static_cast<unsigned>(std::stoi(argv[++i]));
        }
        else {
            print_usage();
            return 1;
        }
    }
    if (data_file.empty() || train_split <= 0.0 || train_split >= 1.0) {
        print_usage();
        return 1;
    }

    // Read CSV
    std::ifstream infile(data_file);
    if (!infile.is_open()) {
        std::cerr << "Error: cannot open data file: " << data_file << "\n";
        return 1;
    }
    std::string line;
    std::vector<std::array<double,6>> features;
    std::vector<double> targets;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string field;
        // Skip vendor and model names
        std::getline(ss, field, ',');
        std::getline(ss, field, ',');

        // Read 6 numeric features
        std::array<double,6> x;
        for (int j = 0; j < 6; ++j) {
            std::getline(ss, field, ',');
            x[j] = std::stod(field);
        }

        // Read PRP value and ignore final ERP column
        std::getline(ss, field, ',');
        double prp = std::stod(field);
        std::getline(ss, field, ','); // ERP (unused)

        features.push_back(x);
        targets.push_back(prp);
    }
    infile.close();
    
    size_t N = targets.size();

        // Normalize features (mean=0, std=1) per column
    std::array<double,6> means = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    std::array<double,6> stddevs = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    // Compute means
    for (const auto& row : features) {
        for (int j = 0; j < 6; ++j) {
            means[j] += row[j];
        }
    }
    for (int j = 0; j < 6; ++j) {
        means[j] /= static_cast<double>(N);
    }

    // Compute stddevs
    for (const auto& row : features) {
        for (int j = 0; j < 6; ++j) {
            double diff = row[j] - means[j];
            stddevs[j] += diff * diff;
        }
    }
    for (int j = 0; j < 6; ++j) {
        stddevs[j] = std::sqrt(stddevs[j] / static_cast<double>(N));
        if (stddevs[j] == 0.0) stddevs[j] = 1.0; // avoid division by zero
    }

    // Apply normalization (in-place)
    for (auto& row : features) {
        for (int j = 0; j < 6; ++j) {
            row[j] = (row[j] - means[j]) / stddevs[j];
        }
    }

    size_t trainN = static_cast<size_t>(train_split * N);
    size_t testN  = N - trainN;

    // Shuffle indices
    std::vector<size_t> idx(N);
    for (size_t i = 0; i < N; ++i) idx[i] = i;
    std::mt19937 rng(seed);
    std::shuffle(idx.begin(), idx.end(), rng);

    // Build design matrices (features + intercept)
    Matrix Xtrain(trainN, 7), Xtest(testN, 7);
    Vector ytrain(trainN), ytest(testN);

    for (size_t i = 0; i < trainN; ++i) {
        auto& row = features[idx[i]];
        for (size_t j = 0; j < 6; ++j)
            Xtrain(i+1, j+1) = row[j];
        Xtrain(i+1, 7) = 1.0;  // intercept
        ytrain[i] = targets[idx[i]];
    }
    for (size_t i = 0; i < testN; ++i) {
        auto& row = features[idx[trainN + i]];
        for (size_t j = 0; j < 6; ++j)
            Xtest(i+1, j+1) = row[j];
        Xtest(i+1, 7) = 1.0;
        ytest[i] = targets[idx[trainN + i]];
    }

    std::cout << "RegressionDemo v1.0\n";
    std::cout << "Loaded " << N << " samples (" << trainN << " train / " << testN << " test)\n\n";

    // Normal equations: A = X^T X, b = X^T y
    Matrix A(7,7);
    Vector b(7);
    for (size_t i = 1; i <= 7; ++i) {
        for (size_t j = 1; j <= 7; ++j) {
            double sum = 0.0;
            for (size_t k = 1; k <= trainN; ++k)
                sum += Xtrain(k,i) * Xtrain(k,j);
            A(i,j) = sum;
        }
        double sum2 = 0.0;
        for (size_t k = 1; k <= trainN; ++k)
            sum2 += Xtrain(k,i) * ytrain[k-1];
        b[i-1] = sum2;
    }

    // Solve
    PosSymLinSystem solver(A, b);
    Vector coeff = solver.Solve();

    // RMSE calculation
    auto compute_rmse = [&](const Matrix& X, const Vector& y, size_t M) {
        double rss = 0.0;
        for (size_t i = 1; i <= M; ++i) {
            double pred = 0.0;
            for (size_t j = 1; j <= 7; ++j)
                pred += X(i,j) * coeff[j-1];
            double err = pred - y[i-1];
            rss += err * err;
        }
        return std::sqrt(rss / M);
    };

    double rmse_train = compute_rmse(Xtrain, ytrain, trainN);
    double rmse_test  = compute_rmse(Xtest,  ytest,  testN);

    // Output results
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Coefficients (x1..x7):\n";
    for (size_t i = 1; i <= 7; ++i)
        std::cout << "  x" << i << " = " << coeff[i-1] << "\n";
    std::cout << "\nTrain RMSE: " << rmse_train << "\n";
    std::cout << "Test  RMSE: " << rmse_test  << "\n";

    return 0;
}
