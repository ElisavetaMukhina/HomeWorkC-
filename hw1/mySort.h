#pragma once
#include <iterator>
#include <functional>
#include <utility>

// Структура точки по требованию задания
struct Point {
    double x;
    double y;
};

// Вспомогательная рекурсивная функция быстрой сортировки
template <typename It, typename Compare>
void quickSortInternal(It first, It last, Compare comp) {
    auto dist = std::distance(first, last);
    if (dist <= 1) {
        return; // Массив из 0 или 1 элемента уже отсортирован
    }

    // Выбираем опорный элемент из середины (защита от деградации на отсортированных массивах)
    auto mid = first;
    std::advance(mid, dist / 2);
    auto pivot = *mid;

    It left = first;
    It right = std::prev(last);

    while (left <= right) {
        while (comp(*left, pivot)) {
            ++left;
        }
        while (comp(pivot, *right)) {
            --right;
        }
        if (left <= right) {
            std::swap(*left, *right);
            ++left;
            --right;
        }
    }

    // Рекурсивно сортируем левую и правую части
    if (first < right + 1) {
        quickSortInternal(first, right + 1, comp);
    }
    if (left < last) {
        quickSortInternal(left, last, comp);
    }
}

// Главная шаблонная функция, требуемая заданием
template <typename It, typename Compare = std::less<typename std::iterator_traits<It>::value_type>>
void mySort(It first, It last, Compare comp = Compare{}) {
    quickSortInternal(first, last, comp);
}