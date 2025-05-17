#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <stdexcept>

/**
 * @brief Dynamic 2-D matrix with heap storage.
 */
class Matrix {
public:
    /** Create rows×cols zeroed matrix. */
    Matrix(std::size_t rows, std::size_t cols);
    /** Deep-copy constructor. */
    Matrix(const Matrix& other);
    /** Free heap storage. */
    ~Matrix();

    /** 1-based bounds-checked element access. */
    double& operator()(std::size_t i, std::size_t j);
    const double& operator()(std::size_t i, std::size_t j) const;

    /** Matrix addition. */
    Matrix operator+(const Matrix& rhs) const;
    /** Matrix×Matrix multiplication. */
    Matrix operator*(const Matrix& rhs) const;
    /** Scalar multiplication. */
    Matrix operator*(double scalar) const;

    /** Determinant (square only). */
    double determinant() const;
    /** Inverse (square only). */
    Matrix inverse() const;
    /** Moore–Penrose pseudo-inverse. */
    Matrix pseudoInverse() const;

    std::size_t rows() const noexcept;
    std::size_t cols() const noexcept;

private:
    std::size_t mRows, mCols;
    double**    mData;
};

#endif // MATRIX_HPP
