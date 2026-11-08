
# 1. Библиотека с матрицей
add_library(hw2_matrix
    DenseMatrix.cpp
)

# Где искать Matrix.h и DenseMatrix.h
target_include_directories(hw2_matrix PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

# 2. Программа проверки корректности
add_executable(hw2_tests
    tests/test_main.cpp
)

target_link_libraries(hw2_tests PRIVATE
    hw2_matrix
)

# 3. Программа измерения скорости
add_executable(hw2_bench
    bench/bench_main.cpp
)

target_link_libraries(hw2_bench PRIVATE
    hw2_matrix
)

# 4. Регистрируем тест
add_test(
    NAME hw2_correctness
    COMMAND hw2_tests
)

# Путь к библиотеке AddressSanitizer для MSVC
if(MSVC)
    set(ASAN_DLL_DIR
        "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/bin/Hostx86/x64"
    )

    set_tests_properties(hw2_correctness PROPERTIES
        ENVIRONMENT_MODIFICATION
        "PATH=path_list_prepend:${ASAN_DLL_DIR}"
    )
endif()

target_compile_definitions(hw2_bench PRIVATE
    HW2_BUILD_CONFIG="$<CONFIG>"
)