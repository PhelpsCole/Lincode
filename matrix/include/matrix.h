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
    Matrix(unsigned long long rows, unsigned long long cols);
    Matrix(unsigned long long rows, unsigned long long cols, std::vector<char> &vec);
    Matrix(const std::vector<std::vector<char>> &vec);
    Matrix(const std::vector<char> &vec, bool isTransposed = false);
    Matrix(unsigned long long rows, unsigned long long cols, std::string &str);
    Matrix(std::string &str, char tabs = '\0', char sep='\n');
    Matrix(const Matrix &) = default;
    ~Matrix() = default;

    Matrix &operator=(const Matrix &) = default;
    Matrix operator+(const Matrix &other) const;
    void operator+=(const Matrix &other);
    Matrix operator*(const Matrix &other) const;
    void operator*=(const Matrix &other);

    char &at(unsigned long long row, unsigned long long col);
    const char &at(unsigned long long row, unsigned long long col) const;
    std::vector<char> &getBasis();
    std::vector<char> col(unsigned long long) const;
    std::vector<char> row(unsigned long long) const;
    unsigned long long rows() const;
    unsigned long long cols() const;
    std::vector<char> matrixToVector() const;
    unsigned long long rank() const;
    std::vector<std::vector<char>> toVectors() const;
    Matrix submatrix(const std::vector<unsigned long long> &, const std::vector<unsigned long long> &) const;

    void printMatrix(const std::string &filename = "NoName", char sep = ' ', std::string end = "\n") const;
    void printMatrixInBlocks(unsigned long long row_num = 1, unsigned long long col_num = 1) const;

    void insertRow(unsigned long long, const std::vector<char> &);
    void insertTriangle(std::vector<char>);
    void T();
    std::vector<unsigned long long> gaussElimination(bool = false, std::vector<unsigned long long> = {});
    void echelon(std::vector<unsigned long long> = {});
    void orthogonal();
    void hadamardProduct(const Matrix &);
    void concatenateByRows(const Matrix &);
    void concatenateByColumns(const Matrix &);
    void convertToBasis();

    std::vector<char> multiplyMatrixByVector(const std::vector<char> &vec) const;
    std::vector<char> multiplyVectorByMatrix(const std::vector<char> &vec) const;

private:
    unsigned long long m_rows;
    unsigned long long m_cols;
    std::vector<char> m_data;
};

Matrix generateRandomMatrix(unsigned long long rows, unsigned long long cols);
Matrix generateRandomNonSingular(unsigned long long rows, unsigned long long cols);
Matrix generateRandomPermutation(unsigned long long n, unsigned long long p);
Matrix permFromVector(const std::vector<unsigned long long> &);
Matrix diag(unsigned long long n, char elem);
Matrix hadamardProductSafe(const Matrix &, const Matrix &);
Matrix hadamardProduct(const Matrix &, const Matrix &);
Matrix hadPower(const Matrix &, size_t);

std::vector<char> solution(Matrix, const std::vector<char> &);

Matrix matrFromFile(const std::string &filename, char col_sep = ' ', char row_sep = '\n');
bool isEqual(const Matrix &, const Matrix &);

} // namespace matrix

#endif // MINE_MATRIX_H