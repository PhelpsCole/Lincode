#include "matrix.h"

namespace matrix {

Matrix::Matrix(size_t rows, size_t cols)
    : m_rows(rows), m_cols(cols), m_data(rows * cols) {}

Matrix::Matrix(size_t rows, size_t cols, std::vector<char> &vec)
    : m_rows(rows), m_cols(cols), m_data(vec) {}


Matrix::Matrix(const std::vector<std::vector<char>> &vec) {
    m_rows = vec.size();
    m_cols = vec[0].size();
    m_data = std::vector<char>();
    for (size_t i = 0; i < m_rows; ++i) {
        if (m_cols != vec[i].size()) {
            throw std::invalid_argument("Incorrect inputted vector set.");
        }
        m_data.insert(m_data.end(), vec[i].begin(), vec[i].end() );
    }
}

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
    m_data.resize(m_cols * m_rows);
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
// NOTE: Correct output only for n х n matrixes
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

} // namespace matrix