#include "sorts.h"
#include <iostream>

bool comparator(const std::vector<char> &row1, const std::vector<char> &row2) {
    if (row1.size() != row2.size()) {
        std::cout << row1.size() << " " << row2.size() << std::endl;
        throw std::invalid_argument("Incorrect rows sizes");
    }
    for (size_t i = 0; i < row1.size(); ++i) {
        if (row1[i] < row2[i]) {
            return true;
        } else if (row1[i] > row2[i]) {
            return false;
        }
    }
    return false;
}

bool comparator2(const unsigned long long &a, const unsigned long long &b) {
    return a <= b;
}

void printVector(const std::vector<unsigned long long> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

void printVector(const std::vector<std::vector<char>> &vv) {
    for (size_t i = 0; i < vv.size(); ++i) {
        for (size_t j = 0; j < vv[i].size(); ++j) {
            std::cout << static_cast<int>(vv[i][j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::vector<unsigned long long> v{1, 7, 2, 3, 2, 8, 100, 4, 7};
    printVector(v);
    algorithms::sorts::selectionSort(v, comparator2);
    printVector(v);
    std::vector<std::vector<char>> vv{{1, 0, 1, 0},
                                      {0, 0, 1, 0},
                                      {1, 1, 1, 0},
                                      {1, 0, 0, 0},
                                      {0, 0, 1, 1},
                                      {0, 0, 0, 1}};
    printVector(vv);
    algorithms::sorts::mergeSort(vv, comparator);
    printVector(vv);
    return 0;
}