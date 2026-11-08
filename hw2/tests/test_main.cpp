#include "DenseMatrix.h"

#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void checkEqual(
    const DenseMatrix& actual,
    const DenseMatrix& expected
) {
    require(
        actual.rows() == expected.rows() &&
        actual.cols() == expected.cols(),
        "Different matrix dimensions"
    );

    const double eps = 1e-9;

    for (std::size_t i = 0; i < actual.rows(); ++i) {
        for (std::size_t j = 0; j < actual.cols(); ++j) {
            require(
                std::abs(actual(i, j) - expected(i, j)) <= eps,
                "Different matrix elements"
            );
        }
    }
}

DenseMatrix randomMatrix(
    std::size_t rows,
    std::size_t cols,
    std::mt19937& generator
) {
    DenseMatrix result(rows, cols);
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            result(i, j) = distribution(generator);
        }
    }

    return result;
}

DenseMatrix identity(std::size_t size) {
    DenseMatrix result(size, size);

    for (std::size_t i = 0; i < size; ++i) {
        result(i, i) = 1.0;
    }

    return result;
}

void checkBoth(
    const DenseMatrix& a,
    const DenseMatrix& b,
    const DenseMatrix& expected
) {
    checkEqual(a.mulNaive(b), expected);
    checkEqual(a.mulOptimized(b), expected);

    // Проверяем виртуальный вызов через базовый класс.
    const Matrix& matrix = a;
    checkEqual(matrix.mul(b), expected);
}

void testSmallExample() {
    DenseMatrix a(2, 3);
    DenseMatrix b(3, 2);

    // A:
    // 1 2 3
    // 4 5 6
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            a(i, j) = static_cast<double>(i * 3 + j + 1);
        }
    }

    // B:
    // 7  8
    // 9  10
    // 11 12
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            b(i, j) = static_cast<double>(i * 2 + j + 7);
        }
    }

    DenseMatrix expected(2, 2);

    expected(0, 0) = 58;
    expected(0, 1) = 64;
    expected(1, 0) = 139;
    expected(1, 1) = 154;

    checkBoth(a, b, expected);
}

void testIdentityAndZero() {
    std::mt19937 generator(42);
    DenseMatrix a = randomMatrix(3, 4, generator);

    // Умножение на единичную справа и слева.
    checkBoth(a, identity(4), a);
    checkBoth(identity(3), a, a);

    // Умножение на нулевую справа и слева.
    checkBoth(a, DenseMatrix(4, 2), DenseMatrix(3, 2));
    checkBoth(DenseMatrix(2, 3), a, DenseMatrix(2, 4));
}

void testInvalidDimensions() {
    DenseMatrix a(2, 3);
    DenseMatrix b(2, 2);

    bool naiveThrew = false;
    bool optimizedThrew = false;
    bool interfaceThrew = false;

    try {
        a.mulNaive(b);
    } catch (const std::invalid_argument&) {
        naiveThrew = true;
    }

    try {
        a.mulOptimized(b);
    } catch (const std::invalid_argument&) {
        optimizedThrew = true;
    }

    try {
        const Matrix& matrix = a;
        matrix.mul(b);
    } catch (const std::invalid_argument&) {
        interfaceThrew = true;
    }

    require(naiveThrew, "Naive version must throw");
    require(optimizedThrew, "Optimized version must throw");
    require(interfaceThrew, "Interface must throw");
}

void testRandomMatrices() {
    std::mt19937 generator(123);

    const std::size_t sizes[][3] = {
        {1, 1, 1},
        {2, 3, 4},
        {7, 5, 9},
        {20, 30, 15},
        {50, 50, 50}
    };

    for (const auto& size : sizes) {
        DenseMatrix a = randomMatrix(size[0], size[1], generator);
        DenseMatrix b = randomMatrix(size[1], size[2], generator);

        checkEqual(a.mulNaive(b), a.mulOptimized(b));
    }
}

int main() {
    try {
        testSmallExample();
        std::cout << "[OK] Small rectangular example\n";

        testIdentityAndZero();
        std::cout << "[OK] Identity and zero matrices\n";

        testInvalidDimensions();
        std::cout << "[OK] Invalid dimensions\n";

        testRandomMatrices();
        std::cout << "[OK] Random matrices\n";

        std::cout << "All tests passed!\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "[FAIL] " << error.what() << "\n";
        return 1;
    }
}