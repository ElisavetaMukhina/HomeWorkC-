#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
#include "mySort.h"


bool comparePoints(const Point& a, const Point& b) {
    double distA = a.x * a.x + a.y * a.y;
    double distB = b.x * b.x + b.y * b.y;
    return distA < distB;
}

void testInts() {
    std::cout << "[RUN] Testing integers..." << std::endl;

    // 1. Пустой массив
    std::vector<int> empty;
    mySort(empty.begin(), empty.end());
    assert(empty.empty());

    // 2. Один элемент
    std::vector<int> single = {42};
    mySort(single.begin(), single.end());
    assert(single.size() == 1 && single[0] == 42);

    // 3. Уже отсортированный
    std::vector<int> sorted = {1, 2, 3, 4, 5};
    mySort(sorted.begin(), sorted.end());
    assert(std::is_sorted(sorted.begin(), sorted.end()));

    // 4. В обратном порядке
    std::vector<int> reversed = {5, 4, 3, 2, 1};
    mySort(reversed.begin(), reversed.end());
    assert(std::is_sorted(reversed.begin(), reversed.end()));

    // 5. Все элементы одинаковые
    std::vector<int> allSame = {7, 7, 7, 7};
    mySort(allSame.begin(), allSame.end());
    assert(std::is_sorted(allSame.begin(), allSame.end()));

    // 6. Отрицательные числа
    std::vector<int> negatives = {-10, 5, -20, 0, 3};
    mySort(negatives.begin(), negatives.end());
    assert(std::is_sorted(negatives.begin(), negatives.end()));

    // 7. Повторяющиеся значения
    std::vector<int> duplicates = {3, 1, 3, 2, 1, 2};
    mySort(duplicates.begin(), duplicates.end());
    assert(std::is_sorted(duplicates.begin(), duplicates.end()));

    std::cout << "[OK] All integer tests passed!" << std::endl;
}

void testPoints() {
    std::cout << "[RUN] Testing Point objects..." << std::endl;

    // Точки с расстояниями: p1 -> 5 (3,4), p2 -> 1 (1,0), p3 -> 0 (0,0)
    std::vector<Point> points = {{3.0, 4.0}, {1.0, 0.0}, {0.0, 0.0}};
    mySort(points.begin(), points.end(), comparePoints);

    assert(comparePoints(points[0], points[1]) || (!comparePoints(points[1], points[0])));
    assert(points[0].x == 0.0 && points[0].y == 0.0); // Ближайшая — (0, 0)
    assert(points[1].x == 1.0 && points[1].y == 0.0); // Следующая — (1, 0)

    std::cout << "[OK] All Point tests passed!" << std::endl;
}

int main() {
    testInts();
    testPoints();
    std::cout << "\n>>> ALL HW1 TESTS PASSED SUCCESSFULLY! <<<\n";
    return 0;
}