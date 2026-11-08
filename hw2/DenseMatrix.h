#pragma once

#include "Matrix.h"

#include <limits>
#include <stdexcept>
#include <vector>

class DenseMatrix : public Matrix {
private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<double> data_;

    static std::size_t checkedSize(
        std::size_t rows,
        std::size_t cols
    ) {
        // Защита от переполнения при вычислении rows * cols.
        if (cols != 0 &&
            rows > std::numeric_limits<std::size_t>::max() / cols) {
            throw std::length_error("Matrix is too large");
        }

        return rows * cols;
    }

    void checkDimensions(const DenseMatrix& other) const {
        if (cols_ != other.rows_) {
            throw std::invalid_argument(
                "Incompatible matrix dimensions"
            );
        }
    }

public:
    // Создаёт матрицу заданного размера, заполненную value.
    DenseMatrix(
        std::size_t rows,
        std::size_t cols,
        double value = 0.0
    )
        : rows_(rows),
          cols_(cols),
          data_(checkedSize(rows, cols), value) {
    }

    std::size_t rows() const override {
        return rows_;
    }

    std::size_t cols() const override {
        return cols_;
    }

    // Доступ для изменения элемента: matrix(i, j) = число.
    double& operator()(std::size_t i, std::size_t j) {
        if (i >= rows_ || j >= cols_) {
            throw std::out_of_range("Matrix index out of range");
        }

        return data_[i * cols_ + j];
    }

    // Доступ для чтения элемента у константной матрицы.
    double operator()(std::size_t i, std::size_t j) const {
        if (i >= rows_ || j >= cols_) {
            throw std::out_of_range("Matrix index out of range");
        }

        return data_[i * cols_ + j];
    }

    // Наивное умножение: i -> j -> k.
    DenseMatrix mulNaive(const DenseMatrix& other) const {
        checkDimensions(other);

        DenseMatrix result(rows_, other.cols_);

        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < other.cols_; ++j) {
                double sum = 0.0;

                for (std::size_t k = 0; k < cols_; ++k) {
                    sum += data_[i * cols_ + k] * other.data_[k * other.cols_ + j];
                }

                result.data_[i * other.cols_ + j] = sum;
            }
        }

        return result;
    }

    // Оптимизированное умножение: i -> k -> j.
    DenseMatrix mulOptimized(const DenseMatrix& other) const {
        checkDimensions(other);

        DenseMatrix result(rows_, other.cols_);

        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t k = 0; k < cols_; ++k) {
                const double aik = data_[i * cols_ + k];

                for (std::size_t j = 0; j < other.cols_; ++j) {
                    result.data_[i * other.cols_ + j] += aik * other.data_[k * other.cols_ + j];
                }
            }
        }

        return result;
    }

    // Основной метод интерфейса использует быстрый вариант.
    DenseMatrix mul(const DenseMatrix& other) const override {
        return mulOptimized(other);
    }
};