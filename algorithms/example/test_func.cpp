#include "algorithms.h"

void testHamming(std::vector<char> v1, std::vector<char> v2) {
    std::cout << "Hamming weight of first vector: " << algorithms::hammingWeight(v1) << std::endl;
    std::cout << "Hamming weight of second vector: " << algorithms::hammingWeight(v2) << std::endl;
    std::cout << "Hamming distance of these vectors: " << algorithms::hammingDistance(v1, v2) << std::endl;
}


int main() {
    testHamming({1, 0, 1, 2, 3}, {1, 0, 0, 2, 2});
    return 0;
}