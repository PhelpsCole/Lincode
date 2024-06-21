#include "algorithms.h"
#include "matrix.h"
#include <iostream>

void testRunner(const std::vector<std::vector<char>> &vv) {
    std::cout << "START testRunner:" << std::endl;
    std::vector<std::vector<unsigned long long>> result = algorithms::classSplitter(vv);
    for (size_t i = 0; i < result.size(); ++i) {
        for (size_t j = 0; j < result[i].size(); ++j) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "END testRunner" << std::endl;
}

int main(int argc, char *argv[]) {
    std::string path;
    size_t iterNum = 1;
    if (argc < 2) {
        std::cout << "Input path to matrix and iterNum" << std::endl;
        return 0;
    } else if (argc == 2) {
        path = argv[1];
    } else if (argc >= 3) {
        path = argv[1];
        iterNum = std::stoi(argv[2]);
    }
    for (size_t i = 0; i < iterNum; ++i) {
        testRunner(matrix::matrFromFile(path).toVectors());
    }
    return 0;
}