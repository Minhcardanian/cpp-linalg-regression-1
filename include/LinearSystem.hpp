#ifndef LINEARSYSTEM_HPP
#define LINEARSYSTEM_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

/**
 * @brief Solve Ax = b using Gaussian elimination with pivoting.
 */
class LinearSystem {
public:
    LinearSystem(const Matrix& A, const Vector& b);
    virtual ~LinearSystem();

    /** @returns solution vector x. */
    virtual Vector Solve() const;

protected:
    std::size_t mSize;
    Matrix      mA;
    Vector      mb;
};

/**
 * @brief Conjugate-Gradient solver for symmetric positive systems.
 */
class PosSymLinSystem : public LinearSystem {
public:
    using LinearSystem::LinearSystem;
    Vector Solve() const override;
};

#endif // LINEARSYSTEM_HPP
