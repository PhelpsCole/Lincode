#include "algorithms.h"
#include <iostream>

void testRunner(unsigned long long p1, unsigned long long p2,
                const std::vector<unsigned long long> &columns) {
    std::cout << "START testRunner:" << std::endl;
    std::vector<std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>>>
    result = algorithms::combinationsOfSeparatingSets(p1, p2, columns);
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << "{";
        for (size_t j = 0; j < result[i].first.size(); ++j) {
            std::cout << result[i].first[j] << " ";
        }
        std::cout << "} {";
        for (size_t j = 0; j < result[i].second.size(); ++j) {
            std::cout << result[i].second[j] << " ";
        }
        std::cout << "}" << std::endl;
    }
    std::cout << "END testRunner" << std::endl;
}

int main(int argc, char *argv[]) {
    size_t iterNum = 1;
    size_t p1 = 2;
    size_t p2 = 5;
    size_t size = 7;
    if (argc < 3) {
        std::cout << "Input args in format: p1 p2 size" << std::endl;
    } else if (argc == 3) {
        p1 = std::stoi(argv[1]);
        p2 = std::stoi(argv[2]);
    } else if (argc >= 4) {
        p1 = std::stoi(argv[1]);
        p2 = std::stoi(argv[2]);
        size = std::stoi(argv[3]);
    }
    std::vector<unsigned long long> columns(size);
    std::iota(columns.begin(), columns.end(), 1);
    for (size_t i = 0; i < iterNum; ++i) {
        testRunner(p1, p2, columns);
    }
    return 0;
}
