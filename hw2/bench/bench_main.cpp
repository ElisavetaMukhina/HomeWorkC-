#include "DenseMatrix.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

DenseMatrix randomMatrix(
    std::size_t size,
    std::mt19937& generator
) {
    DenseMatrix result(size, size);
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = 0; j < size; ++j) {
            result(i, j) = distribution(generator);
        }
    }

    return result;
}

double checksum(const DenseMatrix& matrix) {
    double sum = 0.0;

    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            sum += matrix(i, j);
        }
    }

    return sum;
}

double measure(
    const DenseMatrix& a,
    const DenseMatrix& b,
    bool optimized,
    double& totalChecksum
) {
    const auto start = std::chrono::steady_clock::now();

    DenseMatrix result = optimized ? a.mulOptimized(b) : a.mulNaive(b);

    const auto finish = std::chrono::steady_clock::now();

    // Используем результат, чтобы вычисления не были бесполезными
    // с точки зрения оптимизатора компилятора.
    totalChecksum += checksum(result);

    return std::chrono::duration<double, std::milli>(finish - start).count();
}

int main() {
    std::cout << "Configuration: " << HW2_BUILD_CONFIG << "\n";

#ifdef _MSC_VER
    std::cout << "Compiler: MSVC " << _MSC_VER << "\n";
// #elif defined(__clang__)
//     std::cout << "Compiler: Clang " << __clang_version__ << "\n";
// #elif defined(__GNUC__)
//     std::cout << "Compiler: GCC " << __VERSION__ << "\n";
#endif

    std::cout << "Use Release, optimization enabled, no sanitizer.\n";
    std::cout << "Mean of 3 runs; input generation excluded.\n\n";

    std::cout << std::setw(8) << "Size"
              << std::setw(16) << "Naive ms"
              << std::setw(16) << "Optimized ms"
              << std::setw(12) << "Speedup"
              << "\n";

    std::mt19937 generator(42);
    const std::size_t sizes[] = {100, 200, 500, 1000};
    const int repeats = 3;

    double totalChecksum = 0.0;

    // Небольшой предварительный запуск обеих реализаций.
    DenseMatrix warmup = randomMatrix(50, generator);
    measure(warmup, warmup, false, totalChecksum);
    measure(warmup, warmup, true, totalChecksum);

    for (std::size_t size : sizes) {
        DenseMatrix a = randomMatrix(size, generator);
        DenseMatrix b = randomMatrix(size, generator);

        double naiveMs = 0.0;
        double optimizedMs = 0.0;

        for (int run = 0; run < repeats; ++run) {
            // Чередуем порядок измерений.
            if (run % 2 == 0) {
                naiveMs += measure(a, b, false, totalChecksum);
                optimizedMs += measure(a, b, true, totalChecksum);
            } else {
                optimizedMs += measure(a, b, true, totalChecksum);
                naiveMs += measure(a, b, false, totalChecksum);
            }
        }

        naiveMs /= repeats;
        optimizedMs /= repeats;

        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(8) << size
                  << std::setw(16) << naiveMs
                  << std::setw(16) << optimizedMs
                  << std::setw(12) << naiveMs / optimizedMs
                  << "\n";
    }

    std::cout << "\nChecksum: " << totalChecksum << "\n";
    return 0;
}