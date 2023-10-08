#include "matrix.h"

namespace matrix {

void Matrix::T() {
    std::vector<char> tmp(m_rows * m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            tmp[j * m_rows + i] = m_data[i * m_cols + j];
        }
    }
    m_data = tmp;
    size_t temp = m_cols;
    m_cols = m_rows;
    m_rows = temp;
}

// Do Gauss elimination on matrix
// Returns positions of maximum rank submatrix
// if columns.size() != 0 then apply only on submatrixes on these columns
std::vector<size_t> Matrix::gaussElimination(bool onlyForward, std::vector<size_t> columns) {
    if (m_cols == 0 || m_rows == 0) {
        return std::vector<size_t>();
    }
    size_t edge = columns.size() ? columns.size() : m_cols;
    std::vector<size_t> infoWindow;
    infoWindow.reserve(edge);
    for (size_t i = 0, j = 0; i < m_rows && j < edge; ++j) {
        size_t k = i;
        size_t c = columns.size() == 0 ? j : columns[j];
        bool isZero = true;
        for (; k < m_rows; k++) {
            if (m_data[k * m_cols + c] == 1) {
              isZero = false;
              break;
            }
        }
        if (!isZero) {
            infoWindow.push_back(c);
            if (i != k) {
                for (uint32_t col = 0; col < m_cols; ++col) {
                    m_data[i * m_cols + col] ^= m_data[k * m_cols + col];
                }
            }
            size_t start = 0;
            if (onlyForward) {
                start = i + 1;
            }
            for (uint32_t t = start; t < m_rows; ++t) {
                if (t != i && m_data[t * m_cols + c] == 1) {
                    for (uint32_t col = 0; col < m_cols; ++col) {
                        m_data[t * m_cols + col] ^= m_data[i * m_cols + col];
                    }
                }
            }
            ++i;
        }
    }
    return infoWindow;
}

void Matrix::orthogonal() {
    Matrix diag(*this);
    std::vector<size_t> iw = diag.gaussElimination();
    size_t r = m_cols - iw.size();
    if (r == 0) {
        m_data = std::vector<char>(m_cols);
        m_rows = 1;
    } else {
        std::map<int, char> tmp;
        for (size_t i = 0; i < iw.size(); ++i) {
            tmp[iw[i]] = 0;
        }
        m_data = std::vector<char>(m_cols * r);
        size_t id = 0;
        size_t p = 0;
        for (size_t i = 0; i < m_cols; ++i) {
            if (tmp.find(i) != tmp.end()) {
                for (size_t j = 0, l = 0; l < r; ++j) {
                    if (tmp.find(j) == tmp.end()) {
                        m_data[l * m_cols + i] = diag.at(p, j);
                        ++l;
                    }
                }
                ++p;
            } else {
                m_data[id * m_cols + i] = 1;
                ++id;
            }
        }
        m_rows = r;
    }
}

// Converts matrix to echelon form
void Matrix::echelon(std::vector<size_t> columns) {
    gaussElimination(true, columns);
}

void Matrix::hadamardProduct(const Matrix &other) {
    if (other.rows() != m_rows || other.cols() != m_cols) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_rows; ++j) {
            m_data[i * m_cols + j] &= other.at(i, j);
        }

    }
}

// Not effective by memory
void Matrix::concatenateByRows(const Matrix &second) {
    if (second.rows() != m_rows) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    size_t cols2 = second.cols();
    std::vector<char> v2 = second.matrixToVector();
    std::vector<char>::iterator it1 = m_data.begin(), endIter;
    std::vector<char>::iterator it2 = v2.begin();
    std::vector<char> res;
    for (size_t i = 0; i < m_rows; ++i) {
        endIter = it1 + m_cols;
        res.insert(res.end(), it1, endIter);
        it1 = endIter;
        endIter = it2 + cols2;
        res.insert(res.end(), it2, endIter);
        it2 = endIter;
    }
    m_cols += cols2;
    m_data = res;
}

void Matrix::concatenateByColumns(const Matrix &second) {
    if (second.cols() != m_cols) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    std::vector<char> v2 = second.matrixToVector();
    m_data.insert(m_data.end(), v2.begin(), v2.end());
    m_rows += second.rows();
}

void Matrix::convertToBasis() {
    std::vector<size_t> iw = gaussElimination(true);
    m_rows = iw.size();
    m_data.resize(m_rows * m_cols);
}

} //namespace matrix
