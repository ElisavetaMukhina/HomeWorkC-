#pragma once

#include <cstddef>

// Сообщаем компилятору, что такой класс будет определён позже.
class DenseMatrix;

class Matrix {
public:
    virtual ~Matrix() = default;

    virtual std::size_t rows() const = 0;
    virtual std::size_t cols() const = 0;

    virtual DenseMatrix mul(const DenseMatrix& other) const = 0;
};