#include "matrix.h"

namespace matrix {

Matrix generateRandomMatrix(size_t rows, size_t cols) {
    Matrix m(rows, cols);

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<char> distrib(0, 1);
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            m.at(i, j) = distrib(gen);
        }
    }
    return m;
}

Matrix generateRandomNonSingular(size_t rows, size_t cols) {
    Matrix m = generateRandomMatrix(rows, cols);
    while (m.rank() < std::min(rows, cols)) {
        m = generateRandomMatrix(rows, cols);
    }
    return m;
}

// Ineffective algorithm of finding equiv perms
Matrix generateRandomPermutation(size_t n, size_t p) {
    std::vector<std::vector<char>> vv;
    for (size_t i = 0; i < n; ++i) {
        std::vector<char> tmp(n);
        tmp[i] = 1;
        vv.push_back(tmp);
    }
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<size_t> distrib(0, n - 1);
    size_t k, l;
    std::vector<std::pair<size_t, size_t>> dups;
    for (size_t i = 0; i < p; ++i) {

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

Matrix diag(size_t n, char elem) {
    Matrix D(n, n);
    for (size_t i = 0; i < n; ++i) {
        D.at(i, i) = elem;
    }
    return D;
}

// Returns hadamard product of linear combinations A and B
Matrix hadamardProduct(const Matrix &A, const Matrix &B) {
    if (A.cols() != B.cols()) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    std::vector<char> basis(A.rows() * B.rows() * A.cols());
    size_t k1 = A.rows(), k2 = B.rows(), k12 = A.cols();
    for (size_t i = 0; i < k1; ++i) {
        for (size_t j = 0; j < k2; ++j) {
            for (size_t p = 0; p < k12; ++p) {
                basis[(i * k2 + j) * k12 + p] = A.at(i, p) & B.at(j, p);
            }
        }
    }
    Matrix res(k1 * k2, k12, basis);
    res.convertToBasis();
    return res;
}

Matrix hadPower(const Matrix &m, size_t power) {
    Matrix res(m.cols(), m.rows());
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

// Colculates Ax=B by Gauss-Jordan method
Matrix solution(Matrix &a, Matrix &b) {
    Matrix c(a);
    c.concatenateByRows(b);
    c.gaussElimination();
    c.T();
    std::vector<char> basis = c.matrixToVector();
    size_t size = c.cols() * c.cols();
    std::vector<char> inter_basis(basis.size() - size);
    inter_basis.insert(inter_basis.begin(), basis.begin() + size, basis.end());
    return matrix::Matrix(c.rows() - c.cols(), c.cols(), inter_basis);
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

// {1, 2, 3, 4} -> {1, 2, 3, 4}, p*M
// Sets 0 if not found
Matrix permFromVector(const std::vector<size_t> &v) {
    Matrix perm(v.size(), v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] != 0) {
            perm.at(i, v[i] - 1) = 1;
        }
    }
    return perm;
}
bool isEqual(const Matrix &m1, const Matrix &m2) {
    if (m1.cols() != m2.cols() || m1.rows() != m2.rows()) {
        return false;
    }
    for (size_t i = 0; i < m1.rows(); ++i) {
        for (size_t j = 0; j < m1.cols(); ++j) {
            if (m1.at(i, j) != m2.at(i, j)) {
                return false;
            }
        }
    }
    return true;
}

} // namespace matrix
