#include "Matrix.hpp"
#include <algorithm>

Matrix::Matrix(std::size_t rows, std::size_t cols)
    : mRows(rows), mCols(cols),
      mData(new double*[rows])
{
    for (std::size_t i = 0; i < rows; ++i) {
        mData[i] = new double[cols]();
    }
}

Matrix::Matrix(const Matrix& other)
    : mRows(other.mRows), mCols(other.mCols),
      mData(new double*[other.mRows])
{
    for (std::size_t i = 0; i < mRows; ++i) {
        mData[i] = new double[mCols];
        std::copy(other.mData[i], other.mData[i] + mCols, mData[i]);
    }
}

Matrix::~Matrix() {
    for (std::size_t i = 0; i < mRows; ++i)
        delete[] mData[i];
    delete[] mData;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        Matrix tmp(other);
        std::swap(mRows, tmp.mRows);
        std::swap(mCols, tmp.mCols);
        std::swap(mData, tmp.mData);
    }
    return *this;
}

double& Matrix::operator()(std::size_t i, std::size_t j) {
    if (i == 0 || i > mRows || j == 0 || j > mCols)
        throw std::out_of_range("Matrix 1-based index out of range");
    return mData[i - 1][j - 1];
}

const double& Matrix::operator()(std::size_t i, std::size_t j) const {
    if (i == 0 || i > mRows || j == 0 || j > mCols)
        throw std::out_of_range("Matrix 1-based index out of range");
    return mData[i - 1][j - 1];
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    if (rhs.mRows != mRows || rhs.mCols != mCols)
        throw std::length_error("Matrix size mismatch");
    Matrix out(mRows, mCols);
    for (std::size_t i = 1; i <= mRows; ++i)
        for (std::size_t j = 1; j <= mCols; ++j)
            out(i,j) = (*this)(i,j) + rhs(i,j);
    return out;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    if (mCols != rhs.mRows)
        throw std::length_error("Matrix inner dimensions must agree");
    Matrix out(mRows, rhs.mCols);
    for (std::size_t i = 1; i <= mRows; ++i)
        for (std::size_t j = 1; j <= rhs.mCols; ++j)
            for (std::size_t k = 1; k <= mCols; ++k)
                out(i,j) += (*this)(i,k) * rhs(k,j);
    return out;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix out(mRows, mCols);
    for (std::size_t i = 1; i <= mRows; ++i)
        for (std::size_t j = 1; j <= mCols; ++j)
            out(i,j) = (*this)(i,j) * scalar;
    return out;
}

std::size_t Matrix::rows() const noexcept { return mRows; }
std::size_t Matrix::cols() const noexcept { return mCols; }

// TODO: implement determinant(), inverse(), pseudoInverse()
