#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>

/**
 * @brief Dynamic 1-D vector with heap storage.
 */
class Vector {
public:
    /** Allocate a size-length zeroed vector. */
    explicit Vector(std::size_t size);
    /** Deep-copy constructor. */
    Vector(const Vector& other);
    /** Free heap storage. */
    ~Vector();

    /** Copy-assign. */
    Vector& operator=(const Vector& other);

    /** Unary +. */
    Vector operator+() const;
    /** Unary –. */
    Vector operator-() const;
    /** Element-wise addition. */
    Vector operator+(const Vector& rhs) const;
    /** Scalar multiply. */
    Vector operator*(double scalar) const;

    /** 0-based bounds-checked access. */
    double& operator[](std::size_t idx);
    /** 1-based bounds-checked access. */
    double& operator()(std::size_t idx);

    /** Number of elements. */
    std::size_t size() const noexcept;

private:
    std::size_t mSize;
    double*     mData;
};

#endif // VECTOR_HPP
