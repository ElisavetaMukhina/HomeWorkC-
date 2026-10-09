cmake_minimum_required(VERSION 3.20)
project(CppHomeworks LANGUAGES CXX)

# Включаем современный стандарт языка C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================================
# ДЗ 1 (hw1)
# ==========================================================

# 1. Программа для тестов корректности (папка hw1/tests/)
add_executable(hw1_tests hw1/tests/test_main.cpp)
target_include_directories(hw1_tests PRIVATE hw1)

# Включаем проверку памяти (/fsanitize=address) для Debug-режима
if(MSVC)
    target_compile_options(hw1_tests PRIVATE $<$<CONFIG:Debug>:/fsanitize=address /Zi>)
else()
    target_compile_options(hw1_tests PRIVATE $<$<CONFIG:Debug>:-fsanitize=address -g>)
    target_link_options(hw1_tests PRIVATE $<$<CONFIG:Debug>:-fsanitize=address>)
endif()

# ----------------------------------------------------------

# 2. Программа для замеров скорости (папка hw1/bench/)
add_executable(hw1_bench hw1/bench/bench_main.cpp)
target_include_directories(hw1_bench PRIVATE hw1)

# Включаем максимальную оптимизацию (/O2) для Release-режима (без санитайзеров)
if(MSVC)
    target_compile_options(hw1_bench PRIVATE $<$<CONFIG:Release>:/O2>)
else()
    target_compile_options(hw1_bench PRIVATE $<$<CONFIG:Release>:-O2>)
endif()