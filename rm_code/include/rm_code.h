#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <iostream>
#include <vector>
#include <cmath>

class ReedMullerCode {
public:
    ReedMullerCode(unsigned int r, unsigned int m)
        : r(r), m(m), basis(generateBasis(r, m)) {}

    const std::vector<std::vector<bool>>& getBasis() const {
        return basis;
    }

private:
    unsigned int r;
    unsigned int m;
    std::vector<std::vector<bool>> basis;

    std::vector<std::vector<bool>> generateBasis(unsigned int r, unsigned int m) {
        std::vector<std::vector<bool>> result;
        unsigned int n = static_cast<unsigned int>(std::pow(2, m));

        for (unsigned int weight = m - r + 1; weight <= m; ++weight) {
            for (unsigned int i = 0; i < n; ++i) {
                if (hammingWeight(i) == weight) {
                    result.push_back(decimalToBinaryVector(i, n));
                }
            }
        }

        return result;
    }

    unsigned int hammingWeight(unsigned int x) {
        unsigned int weight = 0;
        while (x) {
            weight += x & 1;
            x >>= 1;
        }
        return weight;
    }

    std::vector<bool> decimalToBinaryVector(unsigned int decimal, unsigned int size) {
        std::vector<bool> binary_vector(size, 0);
        for (unsigned int i = 0; i < size; ++i) {
            binary_vector[i] = (decimal >> (size - 1 - i)) & 1;
        }
        return binary_vector;
    }
};