#include "matrix.h"

namespace matrix {

Matrix Matrix::operator+(const Matrix &other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition.");
    }
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows * m_cols; ++i) {
        result.m_data[i] = m_data[i] ^ other.m_data[i];
    }
    return result;
}

void Matrix::operator+=(const Matrix &other) {
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition.");
    }
    Matrix result(m_rows, m_cols);
    for (size_t i = 0; i < m_rows * m_cols; ++i) {
        m_data[i] ^= other.m_data[i];
    }
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (m_cols != other.m_rows) {
        throw std::invalid_argument("Matrix dimensions must match for multiplication.");
    }
    Matrix result(m_rows, other.m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < other.m_cols; ++j) {
            char sum = 0;
            for (size_t k = 0; k < m_cols; ++k) {
                sum ^= at(i, k) & other.at(k, j);
            }
            result.at(i, j) = sum;
        }
    }
    return result;
}

void Matrix::operator*=(const Matrix &other) {
    if (m_cols != other.m_rows) {
        throw std::invalid_argument("Matrix dimensions must match for multiplication.");
    }
    std::vector<char> result(m_rows * other.m_cols);
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < other.m_cols; ++j) {
            char sum = 0;
            for (size_t k = 0; k < m_cols; ++k) {
                sum ^= at(i, k) & other.at(k, j);
            }
            result[i * other.m_cols + j] = sum;
        }
    }
    m_cols = other.m_cols;
    m_data = result;
}


char& Matrix::at(size_t row, size_t col) {
    return m_data[row * m_cols + col];
}

const char& Matrix::at(size_t row, size_t col) const {
    return m_data[row * m_cols + col];
}
std::vector<char> Matrix::row(size_t i) const {
    std::vector<char>::const_iterator first = m_data.begin() + i * m_cols;
    std::vector<char>::const_iterator last = m_data.begin() + i * m_cols + m_cols;
    return std::vector<char>(first, last);
}
std::vector<char> Matrix::col(size_t j) const {
    std::vector<char> res(m_rows);
    for (size_t i = 0; i < m_rows; ++i) {
        res[i] = m_data[i * m_cols + j];
    }
    return res;
}

std::vector<char> &Matrix::getBasis() {
    return m_data;
}

size_t Matrix::rows() const {
    return m_rows;
}

size_t Matrix::cols() const {
    return m_cols;
}


std::vector<char> Matrix::matrixToVector() const {
    return m_data;
}

size_t Matrix::rank() const {
    Matrix copy(*this);
    return copy.gaussElimination().size();
}

std::vector<std::vector<char>> Matrix::toVectors() const {
    std::vector<std::vector<char>> res(m_rows);
    for (size_t i = 0; i < m_rows; ++i) {
        res[i] = row(i);
    }
    return res;
}

Matrix Matrix::submatrix(const std::vector<size_t> &rows, const std::vector<size_t> &cols) const {
    Matrix res(rows.size(), cols.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        for (size_t j = 0; j < cols.size(); ++j) {
            res.at(i, j) = at(rows[i], cols[j]);
        }
    }
    return res;
}


void Matrix::printMatrix() const {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            std::cout << static_cast<int>(at(i, j)) << " ";
        }
        std::cout << std::endl;
    }
}

void Matrix::printMatrixInBlocks(size_t row_step, size_t col_step) const {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            std::cout << static_cast<int>(at(i, j));
            if (j % col_step == col_step - 1) {
                std::cout << " ";
            }
        }
        if (i % row_step == row_step - 1) {
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
    }
}

std::vector<char> Matrix::multiplyMatrixByVector(const std::vector<char> &vec) const {
    if (m_cols != vec.size()) {
        throw std::invalid_argument("Matrix and vector dimensions must match for multiplication.");
    }

    std::vector<char> result(m_rows);
    for (size_t i = 0; i < m_rows; ++i) {
        char sum = 0;
        for (size_t j = 0; j < m_cols; ++j) {
            sum ^= m_data[i * m_cols + j] & vec[j];
        }
        result[i] = sum;
    }
    return result;
}

std::vector<char> Matrix::multiplyVectorByMatrix(const std::vector<char> &vec) const {
    if (m_rows != vec.size()) {
        throw std::invalid_argument("Matrix and vector dimensions must match for multiplication.");
    }

    std::vector<char> result(m_cols);
    for (size_t j = 0; j < m_cols; ++j) {
        char sum = 0;
        for (size_t i = 0; i < m_rows; ++i) {
            sum ^= vec[i] & m_data[i * m_cols + j];
        }
        result[j] = sum;
    }
    return result;
}

} // namespace matrix