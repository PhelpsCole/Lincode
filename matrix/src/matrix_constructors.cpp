#include "matrix.h"

namespace matrix {

Matrix::Matrix(size_t rows, size_t cols)
    : m_rows(rows), m_cols(cols) {
    for (size_t i = 0; i < m_rows; ++i) {
        m_data.push_back(std::vector<char>(cols));
    }
}

//Matrix::Matrix(size_t rows, size_t cols, std::vector<char> &vec)
//    : m_rows(rows), m_cols(cols) {
//    for (size_t i = 0; i < m_rows; ++i) {
//        std::vector<char>::const_iterator first = vec.begin() + i * m_cols;
//        std::vector<char>::const_iterator last = vec.begin() + i * m_cols + m_cols;
//        m_data.push_back(std::vector<char>(first, last));
//    }
//}

Matrix::Matrix(size_t rows, size_t cols, std::vector<std::vector<char>> vec)
    : m_rows(rows), m_cols(cols), m_data(vec) {
}

Matrix::Matrix(const std::vector<std::vector<char>> &vec) {
    m_rows = vec.size();
    m_cols = vec[0].size();
    for (size_t i = 0; i < m_rows; ++i) {
        if (m_cols != vec[i].size()) {
            throw std::invalid_argument("Incorrect inputted vector set.");
        }
        m_data.push_back(std::vector<char>(vec[i].begin(), vec[i].end()));
    }
}

Matrix::Matrix(std::vector<char> &vec, bool isTransposed) {
    m_cols = vec.size();
    m_rows = 1;
    if (isTransposed) {
        m_rows = m_cols;
        m_cols = 1;
        for (size_t i = 0; i < m_rows; ++i) {
            m_data.push_back(std::vector<char>({vec[i]}));
        }
    } else {
        m_data.push_back(vec);
    }
}


Matrix::Matrix(size_t rows, size_t cols, std::string &str)
    : m_rows(rows), m_cols(cols) {
    for (size_t i = 0; i < m_rows; ++i) {
        std::vector<char> tmp(m_cols);
        for (size_t j = 0; j < m_cols; ++j) {
           tmp[j] = str[i * m_cols + j] - '0';
        }
        m_data.push_back(tmp);
    }
}

Matrix::Matrix(std::string &str, char tabs, char sep) {
    size_t cnt = 0;
    m_cols = 0;
    m_rows = 0;
    std::vector<char> tmp;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == sep) {
            if (!m_cols) {
                m_cols = cnt;
            } else if (m_cols != cnt) {
                throw std::invalid_argument("Bad data in string");
            }
            cnt = 0;
            ++m_rows;
            m_data.push_back(tmp);
            tmp = std::vector<char>(m_cols);
        } else if (str[i] != tabs) {
            if (m_cols == 0) {
                tmp.push_back(str[i] - '0');
            } else {
                tmp[cnt] = str[i] - '0';
            }
            ++cnt;
        }
    }
    if (!m_cols) {
        m_cols = cnt;
    } else if (m_cols != cnt) {
        throw std::invalid_argument("Bad data in string");
    }
    m_data.push_back(tmp);
    ++m_rows;
}

} // namespace matrix
