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

// Returns compbinations of k elements from vector
std::vector<std::vector<size_t>> permsOfVector(const std::vector<size_t> &vec,  size_t k) {
    std::vector<std::vector<size_t>> permSeq = generatePermSequences(vec.size(), k);
    for (size_t i = 0; i < permSeq.size(); ++i) {
        for (size_t j = 0; j < k; ++j) {
            permSeq[i][j] = vec[permSeq[i][j]];
        }
    }
    return permSeq;
}

std::vector<std::vector<size_t>> permsOfVectorWithNew(const std::vector<size_t> &vec,
                                                      size_t newElem, size_t k) {
    std::vector<std::vector<size_t>> permSeq = generatePermSequences(vec.size(), k - 1);
    for (size_t i = 0; i < permSeq.size(); ++i) {
        for (size_t j = 0; j < k; ++j) {
            permSeq[i][j] = vec[permSeq[i][j]];
        }
        permSeq[i].push_back(newElem);
        sort(permSeq[i].begin(), permSeq[i].end());
    }
    return permSeq;
}

std::vector<size_t> sequence(size_t start, size_t end, size_t step) {
    std::vector<size_t> res(end - start);
    res[0] = start;
    for (size_t i = 1; i < end - start; ++i) {
        res[i] += res[i - 1] + step;
    }
    return res;
}

void elemDeleter(std::vector<size_t> &v1, const std::vector<size_t> &v2) {
    size_t ind = 0;
    for (size_t i = 0; i < v1.size(); ++i) {
        bool found = false;
        for (size_t j = 0; j < v2.size(); ++j) {
            if (v1[i] == v2[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            v1[ind++] = v1[i];
        }
    }
    v1.resize(v1.size() - v2.size());
}

} // namespace algorithms