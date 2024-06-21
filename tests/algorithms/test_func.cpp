#include "algorithms.h"
#include <iostream>

void testHamming(std::vector<char> v1, std::vector<char> v2) {
    std::cout << "Hamming weight of first vector: " << algorithms::hammingWeight(v1) << std::endl;
    std::cout << "Hamming weight of second vector: " << algorithms::hammingWeight(v2) << std::endl;
    std::cout << "Hamming distance of these vectors: " << algorithms::hammingDistance(v1, v2) << std::endl;
}

void testFactorial(size_t n) {
    std::cout << "Factorial of " << n << " is " << algorithms::factorial(n) << std::endl;
}

void testGeneratePermSequences(size_t n, size_t k) {
    std::vector<std::vector<unsigned long long>> res = algorithms::generatePermSequences(n, k);
    std::cout << "Sequences of " << k << " elements from set of " << n << " numbers:" << std::endl;
    for (size_t i = 0; i < res.size(); ++i) {
        for (size_t j = 0; j < res[i].size(); ++j) {
            std::cout << res[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void testAddToVectors() {
    std::vector<char> v = {1, 0, 1, 1};
    algorithms::addToBinVector(v, 3);
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << static_cast<int>(v[i]) << " ";
    }
    std::cout << std::endl;
}


int main() {
    testHamming({1, 0, 1, 2, 3}, {1, 0, 0, 2, 2});
    testFactorial(5);
    testGeneratePermSequences(5, 3);
    testGeneratePermSequences(8, 2);
    testAddToVectors();
    return 0;
}