#include "matrix.h"

void testMatrixComputationsAndTranspose(matrix::Matrix &a, matrix::Matrix &b) {
    std::cout << "STARTS testMatrixComputationsAndTranspose" << std::endl;
    std::cout << "Test operations on matrix a and b" << std::endl;
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
    std::cout << "a . b (Hadamard Product)" << std::endl;
    c = a;
    c = hadamardProduct(c, b);
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

void testMatrixSimpleOperations(matrix::Matrix m, unsigned long long row,
                                unsigned long long col) {
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
    for (unsigned long long i = 0; i < m.rows(); ++i) {
        for (unsigned long long j = 0; j < m.cols(); ++j) {
            std::cout << static_cast<int>(vv[i][j]) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Submatr of matrix by {0, 2} rows and {1, 3} cols:" << std::endl;
    matrix::Matrix submatr = m.submatrix({0, 2}, {1, 3});
    submatr.printMatrix();
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

void testGenerators(unsigned long long rows, unsigned long long cols,
                    unsigned long long n, size_t p,
                    std::vector<unsigned long long> v) {
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
    std::cout << "Permutation from vector {";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << ", ";
    }
    std::cout << "}" << std::endl;
    m = matrix::permFromVector(v);
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
    std::vector<unsigned long long> v = copy.gaussElimination();
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

void testSolution(const matrix::Matrix &a, std::vector<char> b) {
    std::cout << "A:" << std::endl;
    a.printMatrix();
    std::cout << "b:" << std::endl;
    for (size_t i = 0; i < b.size(); ++i) {
        std::cout << static_cast<unsigned long long>(b[i]) << std::endl;
    }
    matrix::Matrix c(matrix::solution(a, b), true);
    std::cout << "Solution of Ax = b:" << std::endl;
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
    matrix::Matrix a({{1, 0}, {0, 1}});
    matrix::Matrix b({{1, 1}, {1, 1}});
    testMatrixComputationsAndTranspose(a, b);
    matrix::Matrix m1(code);
    matrix::Matrix m2(str);
    matrix::Matrix m3(code2);
    testMatrixSimpleOperations(m1, 2, 3);
    testGenerators(5, 5, 5, 2, {0, 6, 3, 1, 4, 5, 2});
    testMultiplyByVector(m3, vec2);
    testComplexOperations(m2);
    testSolution(m1, {1, 0, 1, 1});
    code.push_back({1, 0, 1, 0});
    matrix::Matrix m4(code);
    testSolution(m4, {1, 1, 1, 1, 1});
    m4.T();
    return 0;
}