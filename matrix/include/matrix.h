#ifndef MINE_MATRIX_H
#define MINE_MATRIX_H

#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <random>
#include <map>
#include <fstream>


namespace matrix {

class Matrix {
public:
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, std::vector<char> &vec);
    Matrix(const std::vector<std::vector<char>> &vec);
    Matrix(std::vector<char> &vec, bool isTransposed = false);
    Matrix(size_t rows, size_t cols, std::string &str);
    Matrix(std::string &str, char tabs = '\0', char sep='\n');
    Matrix(const Matrix &) = default;
    ~Matrix() = default;

    Matrix &operator=(const Matrix &) = default;
    Matrix operator+(const Matrix &other) const;
    void operator+=(const Matrix &other);
    Matrix operator*(const Matrix &other) const;
    void operator*=(const Matrix &other);

    char &at(size_t row, size_t col);
    const char &at(size_t row, size_t col) const;
    std::vector<char> &getBasis();
    std::vector<char> col(size_t) const;
    std::vector<char> row(size_t) const;
    size_t rows() const;
    size_t cols() const;
    std::vector<char> matrixToVector() const;
    size_t rank() const;
    std::vector<std::vector<char>> toVectors() const;

    void printMatrix() const;
    void printMatrixInBlocks(size_t row_num = 1, size_t col_num = 1) const;

    void T();
    std::vector<size_t> gaussElimination(bool = false, std::vector<size_t> = {});
    void echelon(std::vector<size_t> = {});
    void orthogonal();
    void concatenateByRows(const Matrix &);
    void concatenateByColumns(const Matrix &);
    void convertToBasis();

    std::vector<char> multiplyMatrixByVector(const std::vector<char> &vec) const;
    std::vector<char> multiplyVectorByMatrix(const std::vector<char> &vec) const;

private:
    size_t m_rows;
    size_t m_cols;
    std::vector<char> m_data;
};

Matrix generateRandomMatrix(size_t rows, size_t cols);
Matrix generateRandomNonSingular(size_t rows, size_t cols);
Matrix generateRandomPermutation(size_t n, size_t p);

Matrix solution_transposed(Matrix &a, Matrix &b);
Matrix solution(Matrix &a, Matrix &b);

Matrix matrFromFile(const std::string &filename, char col_sep = ' ', char row_sep = '\n');

} // namespace matrix

#endif // MINE_MATRIX_H