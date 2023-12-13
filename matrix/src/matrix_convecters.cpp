#include "matrix.h"

namespace matrix {

void Matrix::T() {
    std::vector<char> tmp(m_rows * m_cols);
    for (unsigned long long i = 0; i < m_rows; ++i) {
        for (unsigned long long j = 0; j < m_cols; ++j) {
            tmp[j * m_rows + i] = m_data[i * m_cols + j];
        }
    }
    m_data = tmp;
    unsigned long long temp = m_cols;
    m_cols = m_rows;
    m_rows = temp;
}

// Do Gauss elimination on matrix
// Returns positions of maximum rank submatrix
// if columns.size() != 0 then apply only on submatrixes on these columns
std::vector<unsigned long long> Matrix::gaussElimination(bool onlyForward,
                                                         std::vector<unsigned long long> columns) {
    if (m_cols == 0 || m_rows == 0) {
        return std::vector<unsigned long long>();
    }
    unsigned long long edge = columns.size() ? columns.size() : m_cols;
    std::vector<unsigned long long> infoWindow;
    infoWindow.reserve(edge);
    for (unsigned long long i = 0, j = 0; i < m_rows && j < edge; ++j) {
        unsigned long long k = i;
        unsigned long long c = columns.size() == 0 ? j : columns[j];
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
            unsigned long long start = 0;
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
    std::vector<unsigned long long> iw = diag.gaussElimination();
    unsigned long long r = m_cols - iw.size();
    if (r == 0) {
        m_data = std::vector<char>(m_cols);
        m_rows = 1;
    } else {
        std::map<int, char> tmp;
        for (unsigned long long i = 0; i < iw.size(); ++i) {
            tmp[iw[i]] = 0;
        }
        m_data = std::vector<char>(m_cols * r);
        unsigned long long id = 0;
        unsigned long long p = 0;
        for (unsigned long long i = 0; i < m_cols; ++i) {
            if (tmp.find(i) != tmp.end()) {
                for (unsigned long long j = 0, l = 0; l < r; ++j) {
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
void Matrix::echelon(std::vector<unsigned long long> columns) {
    gaussElimination(true, columns);
}

// Not effective by memory
void Matrix::concatenateByRows(const Matrix &second) {
    if (second.rows() != m_rows) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    unsigned long long cols2 = second.cols();
    std::vector<char> v2 = second.matrixToVector();
    std::vector<char>::iterator it1 = m_data.begin(), endIter;
    std::vector<char>::iterator it2 = v2.begin();
    std::vector<char> res;
    for (unsigned long long i = 0; i < m_rows; ++i) {
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
    std::vector<unsigned long long> iw = gaussElimination(true);
    m_rows = iw.size();
    m_data.resize(m_rows * m_cols);
}

} //namespace matrix
