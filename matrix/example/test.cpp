#include "matrix.h"

void testMatrixVectorConstructors(size_t rows, size_t cols, std::vector<char> &basis,
                            std::vector<std::vector<char>> &vec) {
    std::cout << "STARTS testMatrixVectorConstructors" << std::endl;
    std::cout << "Default constructor of " << rows << " x " << cols << " matrix:" << std::endl;
    matrix::Matrix m(rows, cols);
    m.printMatrix();
    std::cout << "Constructor with vector of values:" << std::endl;
    m = matrix::Matrix(rows, cols, basis);
    m.printMatrix();
    std::cout << "Constructor by vector of vectors:" << std::endl;
    m = matrix::Matrix(vec);
    m.printMatrix();
    std::cout << "Constructor by isTransposed vector:" << std::endl;
    m = matrix::Matrix(basis, true);
    m.printMatrix();
}

void testMatrixStringConstructors(size_t rows, size_t cols, std::string &str,
                                  std::string &str2, const char sep = '\n') {
    std::cout << "STARTS testMatrixStringConstructors" << std::endl;
    std::cout << "Constructor by string with matrix sizes:" << std::endl;
    matrix::Matrix m(rows, cols, str);
    m.printMatrix();
    std::cout << "Constructor by string with separator:" << std::endl;
    m = matrix::Matrix(str2, sep);
    m.printMatrix();
}

void testMatrixComputationsAndTranspose(matrix::Matrix &a, matrix::Matrix &b) {
    std::cout << "STARTS testMatrixComputationsAndTranspose" << std::endl;
    std::cout << "Test operations on matrix a and b";
    std::cout << "a:" << std::endl;
    a.printMatrix();
    std::cout << "b:" << std::endl;
    b.printMatrix();
    matrix::Matrix c = b;
    std::cout << "b^T" << std::endl;
    c.T();
    c.printMatrix();
    std::cout << "a + b" << std::endl;
    c = a + b;
    c.printMatrix();
    std::cout << "a * b" << std::endl;
    c = a * b;
    c.printMatrix();

    matrix::Matrix d = c;
    std::cout << "(c|a)" << std::endl;
    d.concatenateByRows(a);
    d.printMatrix();
    std::cout << "(c|b (by columns))" << std::endl;
    d = c;
    d.concatenateByColumns(b);
    d.printMatrix();

    c = a;
    std::cout << "a += b" << std::endl;
    a += b;
    a.printMatrix();
    a = c;
    std::cout << "a *= b" << std::endl;
    a *= b;
    a.printMatrix();
}

void testMatrixSimpleOperations(matrix::Matrix &m, size_t row, size_t col) {
    std::cout << "STARTS testMatrixSimpleOperations" << std::endl;
    std::cout << "Inputed matrix:" << std::endl;
    m.printMatrix();
    std::cout << "Matrix sizes:" << std::endl;
    std::cout << "Rows: " << m.rows() << ", columns: "
              << m.cols() << ", rank: " << m.rank() << std::endl;
    std::cout << "M[" << row << "][" << col << "] = ";
    std::cout << static_cast<int>(m.at(row, col)) << std::endl;
    m.at(row, col) ^= 1;
    std::cout << "Matrix after changed this element:" << std::endl;
    m.printMatrix();
    std::cout << row << " row of matrix:" << std::endl;
    std::vector<char> v = m.row(row);
    for (size_t i = 0 ; i < v.size(); ++i) {
        std::cout << static_cast<int>(v[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << col << " column of matrix:" << std::endl;
    v = m.col(col);
    for (size_t i = 0 ; i < v.size(); ++i) {
        std::cout << static_cast<int>(v[i]) << std::endl;
    }
    v = m.matrixToVector();
    std::cout << "Matrix to simple vector:" << std::endl;
    for (size_t i = 0 ; i < v.size(); ++i) {
        std::cout << static_cast<int>(v[i]) << " ";
    }
    std::cout << std::endl; 
    std::vector<std::vector<char>> vv = m.toVectors();
    std::cout << "Matrix to vector of vectors:" << std::endl;
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            std::cout << static_cast<int>(vv[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

void testMultiplyByVector(matrix::Matrix &m, std::vector<char> &v) {
    std::cout << "STARTS testMultiplyByVector" << std::endl;
    std::cout << "m:" << std::endl;
    m.printMatrix();
    std::cout << "v:" << std::endl;
    for (size_t i = 0 ; i < v.size(); ++i) {
        std::cout << static_cast<int>(v[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "m*v:" << std::endl;
    std::vector<char> res = m.multiplyVectorByMatrix(v);
    for (size_t i = 0 ; i < res.size(); ++i) {
        std::cout << static_cast<int>(res[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "m:" << std::endl;
    m.T();
    m.printMatrix();
    std::cout << "v*m:" << std::endl;
    res = m.multiplyMatrixByVector(v);
    for (size_t i = 0 ; i < res.size(); ++i) {
        std::cout << static_cast<int>(res[i]) << std::endl;
    }
}

void testGenerators(size_t rows, size_t cols, size_t n, size_t p) {
    std::cout << "STARTS testGenerators" << std::endl;
    matrix::Matrix m = matrix::generateRandomMatrix(rows, cols);
    std::cout << "Random matrix:" << std::endl;
    m.printMatrix();
    m = matrix::generateRandomNonSingular(rows, cols);
    std::cout << "Random Non-Singular matrix:" << std::endl;
    m.printMatrix();
    std::cout << "Permutation matrix of " << p << " perms and "
              << n << " size:" << std::endl;
    m = matrix::generateRandomPermutation(n, p);
    m.printMatrix();
}

void testComplexOperations(matrix::Matrix &matrix) {
    std::cout << "STARTS testComplexOperations" << std::endl;
    std::cout << "Inputed matrix:" << std::endl;
    matrix::Matrix copy(matrix);
    matrix.printMatrix();
    std::cout << "Echelon of this matrix:" << std::endl;
    copy.echelon();
    copy.printMatrix();
    copy = matrix;
    std::cout << "Gauss Elimination of this matrix:" << std::endl;
    std::vector<size_t> v = copy.gaussElimination();
    copy.printMatrix();
    std::cout << "Count of Gauss Elimination:" << std::endl;
    for (size_t i = 0 ; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Rank of this matrix: " << matrix.rank() << std::endl;
    std::cout << "Orthogonal of this matrix: " << std::endl;
    matrix::Matrix ort(matrix);
    ort.orthogonal();
    ort.printMatrix();
    ort.T();
    std::cout << "ort^T:" << std::endl;
    ort.printMatrix();
    std::cout << "mat * ort^T = 0:" << std::endl;
    matrix *= ort;
    matrix.printMatrix();
}

void testSolution() {
    std::vector<std::vector<char>> code = {{1, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 1}},
                                   code2 = {{1, 0, 1, 1},
                                            {0, 1, 1, 0}};
    matrix::Matrix a(code);
    matrix::Matrix b(code2);
    b.T();
    matrix::Matrix c = matrix::solution(a, b);
    std::cout << "Solution of Ax = B:" << std::endl;
    c.printMatrix();
}


int main() {
    std::string str = "010100\n101010\n000011";
    std::vector<std::vector<char>> code = {{1, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 1}},
                                   code2 = {{1, 0, 1},
                                            {0, 1, 1}};
    std::vector<char> vec = {1, 0, 0, 1}, vec2 = {1, 0};
    testMatrixVectorConstructors(2, 2, vec, code);
    std::string str2 = "010100101010000011";
    testMatrixStringConstructors(3, 6, str2, str, '\n');
    matrix::Matrix a({{1, 0}, {0, 1}});
    matrix::Matrix b({{1, 1}, {1, 1}});
    testMatrixComputationsAndTranspose(a, b);
    matrix::Matrix m1(code);
    matrix::Matrix m2(str);
    matrix::Matrix m3(code2);
    testMatrixSimpleOperations(m1, 2, 3);
    testGenerators(5, 5, 5, 2);
    testMultiplyByVector(m3, vec2);
    testComplexOperations(m2);
    testSolution();
    return 0;
}