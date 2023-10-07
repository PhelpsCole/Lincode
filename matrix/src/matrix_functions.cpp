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

} // namespace matrix
