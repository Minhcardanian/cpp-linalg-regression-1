// include/Vector.hpp
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>

/**
 * @brief Simple dynamic vector class with heap-managed storage.
 */
class Vector {
public:
    /** Construct a vector of given size, initialized to zero. */
    explicit Vector(std::size_t size);
    /** Copy constructor. */
    Vector(const Vector& other);
    /** Destructor. */
    ~Vector();

    /** Assignment operator. */
    Vector& operator=(const Vector& other);

    /** Unary plus. */
    Vector  operator+() const;
    /** Unary minus. */
    Vector  operator-() const;
    /** Vector addition. */
    Vector  operator+(const Vector& rhs) const;
    /** Scalar multiplication. */
    Vector  operator*(double scalar) const;

    /** 0-based index with bounds check. */
    double&       operator[](std::size_t idx);
    const double& operator[](std::size_t idx) const;

    /** 1-based index with bounds check. */
    double&       operator()(std::size_t idx);
    const double& operator()(std::size_t idx) const;

    /** Return the size of the vector. */
    std::size_t size() const noexcept;

private:
    std::size_t mSize;
    double*     mData;
};

#endif // VECTOR_HPP
