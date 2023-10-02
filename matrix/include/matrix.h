#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

namespace matrix {

class Matrix {
public:
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, std::vector<char> &vec);
    Matrix(const std::vector<std::vector<char>> &vec);
    Matrix(std::vector<char> &vec, bool isTransposed = false);
    Matrix(size_t rows, size_t cols, std::string &str);
    Matrix(std::string &str, char sep='\n');
    Matrix(const Matrix &) = default;
    Matrix &operator=(const Matrix &) = default;
    ~Matrix() = default;

    char &at(size_t row, size_t col);
    const char &at(size_t row, size_t col) const;
    size_t rows() const;
    size_t cols() const;
    std::vector<char> matrixToVector() const;
    Matrix operator+(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;
    void printMatrix();
    void concatenateByRows(const Matrix &);
    void concatenateByColumns(const Matrix &);

    std::vector<char> multiplyMatrixByVector(const std::vector<char> &vec) const;
    std::vector<char> multiplyVectorByMatrix(const std::vector<char> &vec) const;


private:
    size_t m_rows;
    size_t m_cols;
    std::vector<char> m_data;
};


} // namespace matrix