
cmake_minimum_required(VERSION 3.20)

project(Homework LANGUAGES CXX)

# Используем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем систему тестирования
enable_testing()

# Общие настройки компилятора MSVC
if(MSVC)

    # AddressSanitizer несовместим с /RTC
    string(REGEX REPLACE "/RTC[^ ]*" ""
        CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG}"
    )

    # Для санитайзера нужен /Zi вместо /ZI
    string(REPLACE "/ZI" "/Zi"
        CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG}"
    )

    add_compile_options(
        /W4
        /EHsc

        # Debug: поиск ошибок памяти
        "$<$<CONFIG:Debug>:/fsanitize=address>"
        "$<$<CONFIG:Debug>:/Zi>"

        # Release: оптимизация скорости
        "$<$<CONFIG:Release>:/O2>"
    )

    # ASan несовместим с incremental linking
    add_link_options(
        "$<$<CONFIG:Debug>:/INCREMENTAL:NO>"
    )

endif()

# ======================
# Домашнее задание 1
# ======================

add_executable(hw1_tests
    hw1/tests/test_main.cpp
)

add_executable(hw1_bench
    hw1/bench/bench_main.cpp
)

target_include_directories(hw1_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/hw1"
)

target_include_directories(hw1_bench PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/hw1"
)

add_test(NAME hw1_correctness COMMAND hw1_tests)

# ======================
# Домашнее задание 2
# ======================

add_subdirectory(hw2)

# ======================
# Домашнее задание 3
# ======================

# Раскомментируем, когда начнем ДЗ 3
# add_subdirectory(hw3)

# ======================
# Домашнее задание 4
# ======================

# Раскомментируем, когда начнем ДЗ 4
# add_subdirectory(hw4)
