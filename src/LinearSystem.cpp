// src/LinearSystem.cpp
#include "LinearSystem.hpp"
#include <cmath>
#include <stdexcept>
#include <algorithm>

// Constructor: copy A and b with dimension checks
LinearSystem::LinearSystem(const Matrix& A, const Vector& b)
    : mSize(b.size()), mA(A), mb(b)
{
    if (A.rows() != A.cols())
        throw std::invalid_argument("Matrix A must be square");
    if (A.rows() != b.size())
        throw std::invalid_argument("Size mismatch between A and b");
}

LinearSystem::~LinearSystem() = default;

// Gaussian elimination with partial pivoting
Vector LinearSystem::Solve() const {
    std::size_t n = mSize;
    // Make local copies
    Matrix A = mA;
    Vector b = mb;

    // Forward elimination
    for (std::size_t k = 1; k <= n; ++k) {
        // Find pivot row
        std::size_t pivot = k;
        double maxVal = std::abs(A(k, k));
        for (std::size_t i = k + 1; i <= n; ++i) {
            double val = std::abs(A(i, k));
            if (val > maxVal) {
                maxVal = val;
                pivot = i;
            }
        }
        if (maxVal < 1e-12)
            throw std::runtime_error("Matrix is singular or nearly singular");

        // Swap rows in A and entries in b
        if (pivot != k) {
            for (std::size_t j = k; j <= n; ++j)
                std::swap(A(k, j), A(pivot, j));
            std::swap(b[k-1], b[pivot-1]);
        }

        // Eliminate below
        for (std::size_t i = k + 1; i <= n; ++i) {
            double factor = A(i, k) / A(k, k);
            for (std::size_t j = k; j <= n; ++j)
                A(i, j) -= factor * A(k, j);
            b[i-1] -= factor * b[k-1];
        }
    }

    // Back substitution
    Vector x(n);
    for (int i = int(n); i >= 1; --i) {
        double sum = b[i-1];
        for (std::size_t j = i + 1; j <= n; ++j)
            sum -= A(i, j) * x[j-1];
        x[i-1] = sum / A(i, i);
    }

    return x;
}

// Conjugate Gradient for symmetric positive-definite systems
Vector PosSymLinSystem::Solve() const {
    std::size_t n = mSize;
    Vector x(n);           // initial guess = zero
    Vector r = mb;         // residual b - A*x = b
    Vector p = r;          // search direction
    double rsold = 0.0;
    for (std::size_t i = 0; i < n; ++i)
        rsold += r[i] * r[i];

    const double tol = 1e-6;
    const std::size_t maxIter = std::min(n, static_cast<std::size_t>(1000));

    for (std::size_t iter = 0; iter < maxIter; ++iter) {
        // Compute A*p
        Vector Ap(n);
        for (std::size_t i = 1; i <= n; ++i) {
            double dot = 0.0;
            for (std::size_t j = 1; j <= n; ++j)
                dot += mA(i, j) * p[j-1];
            Ap[i-1] = dot;
        }

        // alpha = (rᵀr) / (pᵀAp)
        double pAp = 0.0;
        for (std::size_t i = 0; i < n; ++i)
            pAp += p[i] * Ap[i];
        double alpha = rsold / pAp;

        // Update x and r
        for (std::size_t i = 0; i < n; ++i) {
            x[i] += alpha * p[i];
            r[i] -= alpha * Ap[i];
        }

        // Check convergence
        double rsnew = 0.0;
        for (std::size_t i = 0; i < n; ++i)
            rsnew += r[i] * r[i];
        if (std::sqrt(rsnew) < tol)
            break;

        // Update direction
        for (std::size_t i = 0; i < n; ++i)
            p[i] = r[i] + (rsnew / rsold) * p[i];
        rsold = rsnew;
    }

    return x;
}
