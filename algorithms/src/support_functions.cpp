#include "support_functions.h"

namespace algorithms {

size_t hammingWeight(std::vector<char> v) {
    size_t weight = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] != 0) {
            ++weight;
        }
    }
    return weight;
}

size_t hammingDistance(std::vector<char> v, std::vector<char> v2) {
    if (v.size() != v2.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }
    size_t weight = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] - v2[i] != 0) {
            ++weight;
        }
    }
    return weight;
}

size_t factorial(size_t n) {
    size_t res = 1;
    for (size_t i = 1; i <= n; ++i) {
        res *= i;
    }
    return res;
}

std::vector<size_t> supportGenerator(std::vector<size_t> &s) {
    size_t size = s.size();
    std::vector<size_t> res(size);
    for (size_t i = 0; i < size; ++i) {
        res[i] = s[size - 1 - i] - 1;
    }
    return res;
}


// Slow, need to fix
std::vector<std::vector<size_t>> generatePermSequences(size_t n, size_t k) {
    std::vector<std::vector<size_t>> res;
    std::vector<size_t> seq(k);
    for (size_t i = 0; i < seq.size(); ++i) {
        seq[i] = k - i;
    }
    res.push_back(supportGenerator(seq));
    while(seq[0] != n || seq[k - 1] != n - k + 1) {
        size_t i = 0;
        for (; i < seq.size(); ++i) {
            if (seq[i] < n - i) {
                ++seq[i];
                break;
            }
        }
        size_t ind = i;
        if (ind != 0 && seq[ind - 1] == n - (ind - 1)) {
            do {
                seq[ind - 1] = seq[ind] + 1;
                --ind;
            } while (ind != 0);
        }
        res.push_back(supportGenerator(seq));
    }
    return res;
}

} // namespace algorithms