#include "permutation.h"

namespace permutation {

Permutation::Permutation(unsigned long long len)
    : _len(len), _vec(len), _definedMatrix(false), _matr(len, len) {
    std::iota(_vec.begin(), _vec.end(), 0);
}

Permutation::Permutation(const std::vector<unsigned long long> &vec)
    : _len(vec.size()), _vec(vec), _definedMatrix(false), _matr(vec.size(), vec.size()) {}

Permutation::Permutation(const matrix::Matrix &matr)
    : _len(matr.rows()), _vec(matr.rows()), _definedMatrix(true), _matr(matr) {
    _setVector();
}

Permutation::Permutation(const Permutation &perm)
    : _len(perm._len), _vec(perm._vec), _definedMatrix(perm._definedMatrix), _matr(perm._matr) {}

void Permutation::_setMatrix() {
    if (!_definedMatrix) {
        for (unsigned long long i = 0; i < _vec.size(); ++i) {
            _matr.at(i, _vec[i]) = 1;
        }
        _definedMatrix = true;
    }
}

void Permutation::_setVector() {
    // No checkers. Maybe should be added
    for (unsigned long long i = 0; i < _matr.rows(); ++i) {
        unsigned long long j = 0;
        for (; j < _matr.cols(); ++j) {
            if (_matr.at(i, j) != 0) {
                _vec[i] = j;
                break;
            }
        }
        if (j == _matr.cols()){
            throw std::invalid_argument("Bad inputted matrix.");
        }
    }
}

void Permutation::_clearMatrix() {
    for (unsigned long long i = 0; i < _matr.rows(); ++i) {
        for (unsigned long long j = 0; j < _matr.cols(); ++j) {
            _matr.at(i, j) = 0;
        }
    }
}

Permutation Permutation::operator*(const Permutation &other) const {
    if (_len != other._len) {
        throw std::invalid_argument("Bad vector sizes.");
    }
    std::vector<unsigned long long> ans(_len);
    for (unsigned long long i = 0; i < _len; ++i) {
        ans[i] = other._vec[_vec[i]];
    }
    return ans;

}
void Permutation::operator*=(const Permutation &other) {
    _definedMatrix = false;
    _clearMatrix();
    if (_len != other._len) {
        throw std::invalid_argument("Bad vector sizes.");
    }
    std::vector<unsigned long long> tmp(_len);
    for (unsigned long long i = 0; i < _len; ++i) {
        tmp[i] = other._vec[_vec[i]];
    }
    _vec = tmp;
}
void Permutation::reverse() {
    _definedMatrix = false;
    _clearMatrix();
    std::vector<unsigned long long> tmp(_len);
    for (unsigned long long i = 0; i < _len; ++i) {
        tmp[_vec[i]] = i;
    }
    _vec = tmp;

}
void Permutation::permute(std::vector<unsigned long long> &inp) const {
    if (_len != inp.size()) {
        throw std::invalid_argument("Bad vector size.");
    }
    std::vector<unsigned long long> tmp(inp.size());
    for (unsigned long long i = 0; i < inp.size(); ++i) {
        tmp[i] = inp[_vec[i]];
    }
    inp = _vec;
}
void Permutation::permute(matrix::Matrix &matr) {
    _setMatrix();
    matr *= _matr;
}

unsigned long long Permutation::len() const {
    return _len;
}

std::vector<unsigned long long> Permutation::vec() const {
    return _vec;
}

matrix::Matrix Permutation::matrix() {
    _setMatrix();
    return _matr;
}

// Fisher-Yates algorithm
Permutation generateRandomPermutation(unsigned long long size) {
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::vector<unsigned long long> ans(size);
    std::iota(ans.begin(), ans.end(), 0);
    for (long long i = size - 1; i >= 0; --i) {
        std::uniform_int_distribution<unsigned long long> distrib(0, i);
        long long j = distrib(gen);
        if (i != j) {
            unsigned long long tmp = ans[i];
            ans[i] = ans[j];
            ans[j] = tmp;
        }
    }
    return ans;
}

Permutation generateRandomPermutationOfP(unsigned long long size, unsigned long long p) {
    if (p == size) {
        return generateRandomPermutation(size);
    }
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long> distrib(0, size - 1);

    std::vector<unsigned long long> ans(size);
    std::iota(ans.begin(), ans.end(), 0);
    unsigned long long k, l;
    std::vector<std::pair<unsigned long long, unsigned long long>> dups;

    for (unsigned long long i = 0; i < p; ++i) {
        do {
            k = distrib(gen);
            do {
                l = distrib(gen);
            } while (k == l);
        } while (std::find(dups.begin(), dups.end(), std::make_pair(k, l)) != dups.end());
        dups.push_back(std::make_pair(k, l));
        dups.push_back(std::make_pair(l, k));
        std::iter_swap(ans.begin() + k, ans.begin() + l);
    }
    return ans;
}

} // namespace permutation
