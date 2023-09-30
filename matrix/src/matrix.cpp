#include "matrix.h"

using namespace matrix;

Matrix::Matrix(size_t rows, size_t cols)
    : m_rows(rows), m_cols(cols), m_data(rows * cols) {}

Matrix::Matrix(size_t rows, size_t cols, std::vector<char> &vec)
    : m_rows(rows), m_cols(cols), m_data(vec) {}

Matrix::Matrix(std::vector<char> &vec, bool isTransposed) {
    m_cols = vec.size();
    m_rows = 1;
    m_data = std::vector<char>(m_cols);
    for (size_t i = 0; i < m_cols; ++i) {
        m_data[i] = vec[i];
    }
    if (isTransposed) {
        m_rows = m_cols;
        m_cols = 1;
    }
}

Matrix::Matrix(size_t rows, size_t cols, std::string &str)
    : m_rows(rows), m_cols(cols) {
    m_data = std::vector<char>(str.size());
    std::transform(str.begin(), str.end(), m_data.begin(), [](char c){return c - '0';});
}

Matrix::Matrix(std::string &str, char sep) {
    m_data = std::vector<char>(str.size());
    size_t cnt = 0;
    m_cols = 0;
    m_rows = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == sep) {
            if (!m_cols) {
                m_cols = cnt;
            } else if (m_cols != cnt) {
                throw std::invalid_argument("Bad data in string");
            }
            cnt = 0;
            ++m_rows;
        } else {
            m_data[m_cols * m_rows + cnt] = str[i] - '0';
            ++cnt;
        }
    }
    if (!m_cols) {
        m_cols = cnt;
    } else if (m_cols != cnt) {
        throw std::invalid_argument("Bad data in string");
    }
    ++m_rows;
}

char& Matrix::at(size_t row, size_t col) {
    return m_data[row * m_cols + col];
}

const char& Matrix::at(size_t row, size_t col) const {
    return m_data[row * m_cols + col];
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

void Matrix::printMatrix() {
    for (size_t i = 0; i < m_rows; ++i) {
        for (size_t j = 0; j < m_cols; ++j) {
            std::cout << (char) (at(i, j) + '0') << " ";
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
            sum += vec[i] * m_data[i * m_cols + j];
        }
        result[j] = sum;
    }
    return result;
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