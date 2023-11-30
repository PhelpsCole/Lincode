#include "matrix.h"

namespace matrix {

void Matrix::T() {
    std::vector<std::vector<char>> tmpData;
    for (size_t i = 0; i < m_cols; ++i) {
        std::vector<char> tmp(m_cols);
        for (size_t j = 0; j < m_rows; ++j) {
            tmp[j] = at(j, i);
        }
        tmpData.push_back(tmp);
    }
    m_data = tmpData;
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
            if (at(k, c) == 1) {
              isZero = false;
              break;
            }
        }
        if (!isZero) {
            infoWindow.push_back(c);
            if (i != k) {
                for (uint32_t col = 0; col < m_cols; ++col) {
                    at(i, col) ^= at(k, col);
                }
            }
            size_t start = 0;
            if (onlyForward) {
                start = i + 1;
            }
            for (uint32_t t = start; t < m_rows; ++t) {
                if (t != i && at(t, c) == 1) {
                    for (uint32_t col = 0; col < m_cols; ++col) {
                        at(t, col) ^= at(i, col);
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
        m_data = std::vector<std::vector<char>>();
        m_data.push_back(std::vector<char>(m_cols));
        m_rows = 1;
    } else {
        std::map<int, char> tmp;
        for (size_t i = 0; i < iw.size(); ++i) {
            tmp[iw[i]] = 0;
        }
        for (size_t i = 0; i < r; ++i) {
            m_data.push_back(std::vector<char>(m_cols));
        }
        size_t id = 0;
        size_t p = 0;
        for (size_t i = 0; i < m_cols; ++i) {
            if (tmp.find(i) != tmp.end()) {
                for (size_t j = 0, l = 0; l < r; ++j) {
                    if (tmp.find(j) == tmp.end()) {
                        at(l, i) = diag.at(p, j);
                        ++l;
                    }
                }
                ++p;
            } else {
                at(id, i) = 1;
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

// Not effective by memory
void Matrix::concatenateByRows(const Matrix &second) {
    if (second.rows() != m_rows) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    for(size_t i = 0; i < m_rows; ++i) {
        std::vector<char> secRow = second.row(i);
        m_data[i].insert(m_data[i].end(), secRow.begin(), secRow.end());
    }
    m_cols += second.cols();
}

void Matrix::concatenateByColumns(const Matrix &second) {
    if (second.cols() != m_cols) {
        throw std::invalid_argument("Incorrect inputted matrix.");
    }
    size_t secRows = second.rows();
    for(size_t i = 0; i < secRows; ++i) {
        m_data.push_back(second.row(i));
    }
    m_rows += secRows;
}

void Matrix::convertToBasis() {
    std::vector<size_t> iw = gaussElimination(true);
    m_rows = iw.size();
    m_data.resize(m_rows);
}

} //namespace matrix
