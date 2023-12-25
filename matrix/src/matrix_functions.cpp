#include "matrix.h"

namespace matrix {

Matrix generateRandomMatrix(unsigned long long rows, unsigned long long cols) {
    Matrix m(rows, cols);

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<char> distrib(0, 1);
    for (unsigned long long i = 0; i < m.rows(); ++i) {
        for (unsigned long long j = 0; j < m.cols(); ++j) {
            m.at(i, j) = distrib(gen);
        }
    }
    return m;
}

Matrix generateRandomNonSingular(unsigned long long rows, unsigned long long cols) {
    Matrix m = generateRandomMatrix(rows, cols);
    while (m.rank() < std::min(rows, cols)) {
        m = generateRandomMatrix(rows, cols);
    }
    return m;
}

// Ineffective algorithm of finding equiv perms
Matrix generateRandomPermutation(unsigned long long n, unsigned long long p) {
    std::vector<std::vector<char>> vv;
    for (unsigned long long i = 0; i < n; ++i) {
        std::vector<char> tmp(n);
        tmp[i] = 1;
        vv.push_back(tmp);
    }
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<unsigned long long> distrib(0, n - 1);
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

        iter_swap(vv.begin() + k, vv.begin() + l);
    }
    return Matrix(vv);
}

Matrix diag(unsigned long long n, char elem) {
    Matrix D(n, n);
    for (unsigned long long i = 0; i < n; ++i) {
        D.at(i, i) = elem;
    }
    return D;
}

// Returns hadamard product of linear combinations A and B
Matrix hadamardProduct(const Matrix &A, const Matrix &B) {
    if (A.cols() != B.cols()) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    unsigned long long k1 = A.rows(), k2 = B.rows(), k12 = A.cols();
    Matrix res(0, 0);
    std::vector<char> tmp(k12);
    for (unsigned long long i = 0; i < k1; ++i) {
        for (unsigned long long j = 0; j < k2; ++j) {
            for (unsigned long long p = 0; p < k12; ++p) {
                tmp[p] = A.at(i, p) & B.at(j, p);
            }
            res.insertTriangle(tmp);
        }
    }
    return res;
}

Matrix hadPower(const Matrix &m, size_t power) {
    if (power == 0) {
        return Matrix(std::vector<char>(m.cols(), 1));
    }
    if (power == 1) {
        return m;
    }
    Matrix res(0, 0);
    bool resEmpty = true;
    Matrix tmp(m);
    while(power) {
        if (power % 2 == 1) {
            if (resEmpty) {
                res = tmp;
                resEmpty = false;
            } else {
                res = hadamardProduct(res, tmp);
            }
        }
        power >>= 1;
        if (power) {
            tmp = hadamardProduct(tmp, tmp);
        }
    }
    return res;
}

// Colculates Ax=b by Gauss-Jordan method
std::vector<char> solution(Matrix a, const std::vector<char> &b) {
    if (a.rows() < a.cols()) {
        std::cerr << a.rows() << " " << a.cols() << std::endl;
        throw std::invalid_argument("This function didn't calculate fundamental system");
    }
    a.concatenateByRows(Matrix(b, true));
    a.gaussElimination(true);
    std::vector<char> res(a.cols() - 1);
    for (int i = res.size() - 1; i >= 0; --i) {
        res[i] = a.at(i, a.cols() - 1); // = b'[i];
        for (size_t j = i + 1; j < res.size(); ++j) {
            if (a.at(i, j) != 0) {
                res[i] ^= res[j];
            }
        }
    }
    return res;
}

Matrix matrFromFile(const std::string& filename, char col_sep, char row_sep) {
        std::string str, tmp;
    std::ifstream input_file(filename);
    if (input_file.is_open()) {
        while(getline(input_file, tmp)) {
            str += tmp + row_sep;
        }
        return Matrix(str, col_sep, row_sep);
    } else {
        std::cerr << "Unable to open the file: " << filename << std::endl;
        return Matrix(1, 1);
    }
}

// {0, 1, 2, 3, 4} -> {0, 1, 2, 3, 4}, p*M
Matrix permFromVector(const std::vector<unsigned long long> &v, bool transpose) {
    Matrix perm(v.size(), v.size());
    for (unsigned long long i = 0; i < v.size(); ++i) {
        if (transpose) {
            perm.at(v[i], i) = 1;
        } else {
            perm.at(i, v[i]) = 1;
        }
    }
    return perm;
}
bool isEqual(const Matrix &m1, const Matrix &m2) {
    if (m1.cols() != m2.cols() || m1.rows() != m2.rows()) {
        return false;
    }
    for (unsigned long long i = 0; i < m1.rows(); ++i) {
        for (unsigned long long j = 0; j < m1.cols(); ++j) {
            if (m1.at(i, j) != m2.at(i, j)) {
                return false;
            }
        }
    }
    return true;
}

} // namespace matrix
