#include "LinearSystem.hpp"

// Base ctor
LinearSystem::LinearSystem(const Matrix& A, const Vector& b)
    : mSize(b.size()), mA(A), mb(b)
{}

LinearSystem::~LinearSystem() = default;

Vector LinearSystem::Solve() const {
    // TODO: Gaussian elimination with partial pivoting
    return Vector(mSize);
}

Vector PosSymLinSystem::Solve() const {
    // TODO: Conjugate-gradient algorithm
    return Vector(mSize);
}
