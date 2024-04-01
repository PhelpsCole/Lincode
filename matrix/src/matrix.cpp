#include "matrix.h"

namespace matrix {

Matrix Matrix::operator+(const Matrix &other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition.");
    }
    Matrix result(m_rows, m_cols);
    for (unsigned long long i = 0; i < m_rows * m_cols; ++i) {
        result.m_data[i] = m_data[i] ^ other.m_data[i];
    }
    return result;
}

void Matrix::operator+=(const Matrix &other) {
    if (m_rows != other.m_rows || m_cols != other.m_cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition.");
    }
    Matrix result(m_rows, m_cols);
    for (unsigned long long i = 0; i < m_rows * m_cols; ++i) {
        m_data[i] ^= other.m_data[i];
    }
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (m_cols != other.m_rows) {
        throw std::invalid_argument("Matrix dimensions must match for multiplication.");
    }
    Matrix result(m_rows, other.m_cols);
    for (unsigned long long i = 0; i < m_rows; ++i) {
        for (unsigned long long j = 0; j < other.m_cols; ++j) {
            char sum = 0;
            for (unsigned long long k = 0; k < m_cols; ++k) {
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
    for (unsigned long long i = 0; i < m_rows; ++i) {
        for (unsigned long long j = 0; j < other.m_cols; ++j) {
            char sum = 0;
            for (unsigned long long k = 0; k < m_cols; ++k) {
                sum ^= at(i, k) & other.at(k, j);
            }
            result[i * other.m_cols + j] = sum;
        }
    }
    m_cols = other.m_cols;
    m_data = result;
}

void Matrix::addRow(unsigned long long rowInd, const std::vector<char> &row) {
    for (unsigned long long j = 0; j < row.size(); ++j) {
        at(rowInd, j) ^= row[j];
    }
}


char& Matrix::at(unsigned long long row, unsigned long long col) {
    return m_data[row * m_cols + col];
}

const char& Matrix::at(unsigned long long row, unsigned long long col) const {
    return m_data[row * m_cols + col];
}

std::vector<char> Matrix::row(unsigned long long i) const {
    std::vector<char>::const_iterator first = m_data.begin() + i * m_cols;
    std::vector<char>::const_iterator last = m_data.begin() + i * m_cols + m_cols;
    return std::vector<char>(first, last);
}

std::vector<char> Matrix::col(unsigned long long j) const {
    std::vector<char> res(m_rows);
    for (unsigned long long i = 0; i < m_rows; ++i) {
        res[i] = m_data[i * m_cols + j];
    }
    return res;
}

std::vector<char> &Matrix::getBasis() {
    return m_data;
}

unsigned long long Matrix::rows() const {
    return m_rows;
}

unsigned long long Matrix::cols() const {
    return m_cols;
}


std::vector<char> Matrix::matrixToVector() const {
    return m_data;
}

unsigned long long Matrix::rank() const {
    Matrix copy(*this);
    return copy.gaussElimination().size();
}

std::vector<std::vector<char>> Matrix::toVectors() const {
    std::vector<std::vector<char>> res(m_rows);
    for (unsigned long long i = 0; i < m_rows; ++i) {
        res[i] = row(i);
    }
    return res;
}

Matrix Matrix::submatrix(const std::vector<unsigned long long> &rows,
                         const std::vector<unsigned long long> &cols) const {
    Matrix res(rows.size(), cols.size());
    for (size_t i = 0; i < rows.size(); ++i) {
        for (size_t j = 0; j < cols.size(); ++j) {
            res.at(i, j) = at(rows[i], cols[j]);
        }
    }
    return res;
}


void Matrix::printMatrix(const std::string &filename, char sep, std::string end) const {
    if (filename == "NoName") {
        for (unsigned long long i = 0; i < m_rows; ++i) {
            for (unsigned long long j = 0; j < m_cols; ++j) {
                std::cout << static_cast<int>(at(i, j)) << sep;
            }
            std::cout << end;
        }
        return;
    }
    std::ofstream out;
    out.open(filename);
    for (unsigned long long i = 0; i < m_rows; ++i) {
        for (unsigned long long j = 0; j < m_cols; ++j) {
            out << static_cast<int>(at(i, j)) << sep;
        }
        out << end;
    }
    out.close();
}

void Matrix::printVisualMatrix(unsigned long long blocks_num,
                               bool printWeight,
                               const std::string &filename,
                               std::string end) const {
    if (filename == "NoName") {
        for (unsigned long long i = 0; i < m_rows; ++i) {
            for (unsigned long long j = 0; j < m_cols; ++j) {
                if (j && j % (m_cols / blocks_num) == 0) {
                    std::cout << "|";
                }
                if (at(i, j) == 0) {
                    std::cout << "-";
                } else if (at(i, j) == 1) {
                    std::cout << "+";
                }
            }
            if (printWeight) {
                std::cout << " [" << algorithms::hammingWeight(row(i)) << "]";
            }
            std::cout << end;
        }
        return;
    }
    std::ofstream out;
    out.open(filename);
    out << "m_rows = " << m_rows << ", m_cols = " << m_cols << std::endl;
    for (unsigned long long i = 0; i < m_rows; ++i) {
        for (unsigned long long j = 0; j < m_cols; ++j) {
            if (j && j % (m_cols / blocks_num) == 0) {
                out << "|";
            }
            if (at(i, j) == 0) {
                out << "-";
            } else if (at(i, j) == 1) {
                out << "+";
            }
        }
        out << end;
    }
    out.close();
}

void Matrix::printMatrixInBlocks(unsigned long long row_step, unsigned long long col_step) const {
    for (unsigned long long i = 0; i < m_rows; ++i) {
        for (unsigned long long j = 0; j < m_cols; ++j) {
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
    for (unsigned long long i = 0; i < m_rows; ++i) {
        char sum = 0;
        for (unsigned long long j = 0; j < m_cols; ++j) {
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
    for (unsigned long long j = 0; j < m_cols; ++j) {
        char sum = 0;
        for (unsigned long long i = 0; i < m_rows; ++i) {
            sum ^= vec[i] & m_data[i * m_cols + j];
        }
        result[j] = sum;
    }
    return result;
}

} // namespace matrix