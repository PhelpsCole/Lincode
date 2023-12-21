#include "matrix.h"

namespace matrix {

Matrix::Matrix(unsigned long long rows, unsigned long long cols)
    : m_rows(rows), m_cols(cols), m_data(rows * cols) {}

Matrix::Matrix(unsigned long long rows, unsigned long long cols, std::vector<char> &vec)
    : m_rows(rows), m_cols(cols), m_data(vec) {}


Matrix::Matrix(const std::vector<std::vector<char>> &vec) {
    m_rows = vec.size();
    m_cols = vec[0].size();
    m_data = std::vector<char>();
    for (unsigned long long i = 0; i < m_rows; ++i) {
        if (m_cols != vec[i].size()) {
            throw std::invalid_argument("Incorrect inputted vector set.");
        }
        m_data.insert(m_data.end(), vec[i].begin(), vec[i].end() );
    }
}

Matrix::Matrix(const std::vector<char> &vec, bool isTransposed) {
    m_cols = vec.size();
    m_rows = 1;
    m_data = std::vector<char>(m_cols);
    for (unsigned long long i = 0; i < m_cols; ++i) {
        m_data[i] = vec[i];
    }
    if (isTransposed) {
        m_rows = m_cols;
        m_cols = 1;
    }
}

Matrix::Matrix(unsigned long long rows, unsigned long long cols, std::string &str)
    : m_rows(rows), m_cols(cols) {
    m_data = std::vector<char>(str.size());
    std::transform(str.begin(), str.end(), m_data.begin(), [](char c){return c - '0';});
}

Matrix::Matrix(std::string &str,  char tabs, char sep) {
    m_data = std::vector<char>(str.size());
    unsigned long long cnt = 0;
    m_cols = 0;
    m_rows = 0;
    for (unsigned long long i = 0; i < str.size(); ++i) {
        if (str[i] == sep) {
            if (!m_cols) {
                m_cols = cnt;
            } else if (m_cols != cnt) {
                std::cout << m_cols << " " << cnt << " " << i << std::endl;
                throw std::invalid_argument("Bad data in string");
            }
            cnt = 0;
            ++m_rows;
        } else if (str[i] != tabs) {
            m_data[m_cols * m_rows + cnt] = str[i] - '0';
            ++cnt;
        }
    }
    if (!m_cols) {
        m_cols = cnt;
    } else if (m_cols != cnt && cnt != 0) {
        throw std::invalid_argument("Bad data in string");
    } else if (cnt != 0) {
        ++m_rows;
    }
    m_data.resize(m_cols * m_rows);
}

} // namespace matrix
