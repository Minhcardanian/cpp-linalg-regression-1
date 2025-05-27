// src/Vector.cpp
#include "Vector.hpp"
#include <algorithm>
#include <stdexcept>

Vector::Vector(std::size_t size)
    : mSize(size),
      mData(new double[size]())
{}

Vector::Vector(const Vector& other)
    : mSize(other.mSize),
      mData(new double[other.mSize])
{
    std::copy(other.mData, other.mData + mSize, mData);
}

Vector::~Vector() {
    delete[] mData;
}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        if (mSize != other.mSize) {
            delete[] mData;
            mSize = other.mSize;
            mData = new double[mSize];
        }
        std::copy(other.mData, other.mData + mSize, mData);
    }
    return *this;
}

Vector Vector::operator+() const {
    return *this;
}

Vector Vector::operator-() const {
    Vector tmp(*this);
    for (std::size_t i = 0; i < mSize; ++i)
        tmp.mData[i] = -tmp.mData[i];
    return tmp;
}

Vector Vector::operator+(const Vector& rhs) const {
    if (rhs.mSize != mSize)
        throw std::length_error("Vector size mismatch in addition");
    Vector out(mSize);
    for (std::size_t i = 0; i < mSize; ++i)
        out.mData[i] = mData[i] + rhs.mData[i];
    return out;
}

Vector Vector::operator*(double scalar) const {
    Vector out(mSize);
    for (std::size_t i = 0; i < mSize; ++i)
        out.mData[i] = mData[i] * scalar;
    return out;
}

double& Vector::operator[](std::size_t idx) {
    if (idx >= mSize) throw std::out_of_range("Vector index out of range");
    return mData[idx];
}

const double& Vector::operator[](std::size_t idx) const {
    if (idx >= mSize) throw std::out_of_range("Vector index out of range");
    return mData[idx];
}

double& Vector::operator()(std::size_t idx) {
    if (idx == 0 || idx > mSize) throw std::out_of_range("Vector 1-based index out of range");
    return mData[idx - 1];
}

const double& Vector::operator()(std::size_t idx) const {
    if (idx == 0 || idx > mSize) throw std::out_of_range("Vector 1-based index out of range");
    return mData[idx - 1];
}

std::size_t Vector::size() const noexcept {
    return mSize;
}
