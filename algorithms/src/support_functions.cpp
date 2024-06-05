#include "support_functions.h"
#include "sorts.h"

namespace algorithms {

size_t hammingWeight(const std::vector<char> &v) {
    size_t weight = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] != 0) {
            ++weight;
        }
    }
    return weight;
}

size_t hammingDistance(const std::vector<char> &v, const std::vector<char> &v2) {
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

size_t gcd(size_t x, size_t y) {
    while (x != y) {
        if (x > y) {
            x -= y;
        } else {
            y -= x;
        }
    }
    return x;
}

// Returns gcd and a, b: ax + by = gcd
std::vector<int> extendedGcd(int x, int y) {
    int prev_a = 1, a = 0;
    int prev_b = 0, b = 1;
    while (y != 0) {
        int q = x / y;
        int r = x % y;
        int tmp = a;
        a = prev_a - q * a;
        prev_a = tmp;
        tmp = b;
        b = prev_b - q * b;
        prev_b = tmp;
        x = y;
        y = r;
    }
    return {x, prev_a, prev_b};
}

std::vector<unsigned long long> supportGenerator(const std::vector<unsigned long long> &s) {
    size_t size = s.size();
    std::vector<unsigned long long> res(size);
    for (size_t i = 0; i < size; ++i) {
        res[i] = s[size - 1 - i] - 1;
    }
    return res;
}

// Generate seq from 1 to max of size size
std::vector<unsigned long long> generateCols(unsigned long long max, unsigned long long size) {
    std::vector<unsigned long long> res(size);
    std::vector<unsigned long long> unused(max, 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long> distrib(0, max - 1);
    for (size_t i = 0; i < res.size(); ++i) {
        unsigned long long ind = distrib(gen);
        while (!unused[ind]) {
            ind = distrib(gen);
        }
        unused[ind] = 0;
        res[i] = ind;
    }
    /*algorithms::sorts::mergeSort(res, [](const unsigned long long &a,
                                         const unsigned long long &b)
                                         { return a <= b; });*/
    return res;
}

// Slow, need to fix
std::vector<std::vector<unsigned long long>> generatePermSequences(size_t n, size_t k) {
    std::vector<std::vector<unsigned long long>> res;
    std::vector<unsigned long long> seq(k);
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
std::vector<std::vector<unsigned long long>> permsOfVector(const std::vector<size_t> &vec,  size_t k) {
    std::vector<std::vector<unsigned long long>> permSeq = generatePermSequences(vec.size(), k);
    for (size_t i = 0; i < permSeq.size(); ++i) {
        for (size_t j = 0; j < k; ++j) {
            permSeq[i][j] = vec[permSeq[i][j]];
        }
    }
    return permSeq;
}

std::vector<std::vector<unsigned long long>> permsOfVectorWithNew(const std::vector<size_t> &vec,
                                                      size_t newElem, size_t k) {
    std::vector<std::vector<unsigned long long>> permSeq = generatePermSequences(vec.size(), k - 1);
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

void addToBinVector(std::vector<char> &v, size_t n) {
    for (size_t i = 0; i < v.size() && n != 0; ++i) {
        v[i] += n & 1;
        if (v[i] == 2) {
            v[i] = 0;
            ++n;
        }
        n >>= 1;
    }
    if (n != 0) {
        int cnt = 0, prev_size = v.size();
        int tmp = n;
        while (tmp) {
            ++cnt;
            tmp >>= 1;
        }
        v.resize(prev_size + cnt);
        for (size_t i = 0; i < v.size() - prev_size; ++i) {
            v[prev_size + i] = n & 1;
            n >>= 1;
        }
    }
}

void addToBinWeightVector(std::pair<std::vector<char>, unsigned long long> &v, size_t n) {
    for (size_t i = 0; i < v.first.size() && n != 0; ++i) {
        v.first[i] += n & 1;
        if (v.first[i] == 2) {
            v.first[i] = 0;
            ++n;
            --v.second;
        } else {
            ++v.second;
        }
        n >>= 1;
    }
    if (n != 0) {
        int cnt = 0, prev_size = v.first.size();
        int tmp = n;
        while (tmp) {
            ++cnt;
            tmp >>= 1;
        }
        v.first.resize(prev_size + cnt);
        for (size_t i = 0; i < v.first.size() - prev_size; ++i) {
            v.first[prev_size + i] = n & 1;
            if (v.first[prev_size + i]) {
                ++v.second;
            }
            n >>= 1;
        }
    }
}

bool isIntersected(const std::vector<unsigned long long> &a, const std::vector<unsigned long long> &b) {
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b.size(); ++j) {
            if (a[i] == b[j]) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::vector<unsigned long long>>
classSplitter(const std::vector<std::vector<char>> &vv,
              const std::vector<unsigned long long> &start_class) {
    std::vector<std::vector<unsigned long long>> classes;
    if (start_class.size() != 0) {
        classes.push_back(start_class);
    }
    for (unsigned long long i = 0; i < vv.size(); ++i) {
        std::vector<unsigned long long> newClass;
        for (unsigned long long j = 0; j < vv[i].size(); ++j) {
            if (vv[i][j] == 1) {
                newClass.push_back(j);
            }
        }
        bool isIntersected = false;
        std::vector<size_t> intersectedIndexes;
        for (size_t k = 0; k < classes.size(); ++k) {
            isIntersected = algorithms::isIntersected(classes[k], newClass);
            if (isIntersected) {
                intersectedIndexes.push_back(k);
            }
        }
        if (!isIntersected) {
            classes.push_back(newClass);
        } else if (intersectedIndexes.size() == 1) {
            for (size_t n = 0; n < newClass.size(); ++n) {
                bool insideClass = false;
                for (size_t m = 0; m < classes[intersectedIndexes[0]].size(); ++m) {
                    if (classes[intersectedIndexes[0]][m] == newClass[n]) {
                        insideClass = true;
                        break;
                    }
                }
                if (!insideClass) {
                    classes[intersectedIndexes[0]].push_back(newClass[n]);
                }
            }
        } else {
            size_t ind = 0;
            for (size_t k = 0; k < classes.size(); ++k) {
                if (ind != intersectedIndexes.size() && intersectedIndexes[ind] == k) {
                    std::vector<unsigned long long> tmp;
                    for (size_t m = 0; m < classes[k].size(); ++m) {
                        bool insideClass = false;
                        for (size_t n = 0; n < newClass.size(); ++n) {
                            if (classes[k][m] == newClass[n]) {
                                insideClass = true;
                                break;
                            }
                        }
                        if (!insideClass) {
                            tmp.push_back(classes[k][m]);
                        }
                    }
                    newClass.insert(newClass.end(), tmp.begin(), tmp.end());
                    ++ind;
                } else if (ind != 0) {
                    classes[k - ind] = classes[k];
                }
            }
            classes.resize(classes.size() - ind);
            classes.push_back(newClass);
        }
    }
    for (size_t i = 0; i < classes.size(); ++i) {
        algorithms::sorts::mergeSort(classes[i], [](const unsigned long long &a,
                                                    const unsigned long long &b)
                                                 { return a <= b; });
    }
    return classes;
}

// Returns all combinations of separating vector to two sets of sizes p1 and p2
std::vector<std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>>>
combinationsOfSeparatingSets(unsigned long long p1, unsigned long long p2,
                             std::vector<unsigned long long> columns) {
    std::vector<std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>>>
    result;
    result.push_back(std::make_pair(std::vector<unsigned long long>({}), std::vector<unsigned long long>({})));
    for (unsigned long long i = 0; i < columns.size(); ++i) {
        std::vector<std::pair<std::vector<unsigned long long>, std::vector<unsigned long long>>>
        tmpResult;
        for (unsigned long long j = 0; j < result.size(); ++j) {
            if (result[j].first.size() != p1 && result[j].second.size() != p2) {
                std::vector<unsigned long long> tmp1(result[j].first);
                std::vector<unsigned long long> tmp2(result[j].second);
                std::vector<unsigned long long> tmp(tmp1);
                tmp.push_back(columns[i]);
                tmpResult.push_back(std::make_pair(tmp, tmp2));
                tmp = tmp2;
                tmp.push_back(columns[i]);
                tmpResult.push_back(std::make_pair(tmp1, tmp));
            } else if (result[j].first.size() != p1) {
                result[j].first.push_back(columns[i]);
                tmpResult.push_back(result[j]);
            } else if (result[j].second.size() != p2) {
                result[j].second.push_back(columns[i]);
                tmpResult.push_back(result[j]);
            }
        }
        result = tmpResult;
    }
    return result;
}

} // namespace algorithms