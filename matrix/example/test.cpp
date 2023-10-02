#include "matrix.h"

int main() {
    matrix::Matrix a(2, 2);
    a.at(0, 0) = 1;
    a.at(0, 1) = 0;
    a.at(1, 0) = 0;
    a.at(1, 1) = 1;

    matrix::Matrix b(2, 2);
    b.at(0, 0) = 1;
    b.at(0, 1) = 1;
    b.at(1, 0) = 1;
    b.at(1, 1) = 1;

    matrix::Matrix c = a + b;
    matrix::Matrix d = a * b;

    std::vector<std::vector<char>> code = {{1, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 1}};
    matrix::Matrix mFromCode = matrix::Matrix(code);
    std::cout << "Matrix from vector of vectors:" << std::endl;
    mFromCode.printMatrix();

    std::cout << "Matrix c (a + b):" << std::endl;
    c.printMatrix();

    std::cout << "Matrix d (a * b):" << std::endl;
    /*for (size_t i = 0; i < d.rows(); ++i) {
        for (size_t j = 0; j < d.cols(); ++j) {
            std::cout << d.at(i, j) << " ";
        }
        std::cout << std::endl;
    }*/
    d.printMatrix();

    matrix::Matrix e = c;
    e.concatenateByRows(d);
    matrix::Matrix f = c;
    f.concatenateByColumns(d);
    std::cout << "Matrix e (c|d):" << std::endl;
    e.printMatrix();
    std::cout << "Matrix e (c|d (by columns)):" << std::endl;
    f.printMatrix();


    matrix::Matrix mat(2, 3);
    mat.at(0, 0) = 0;
    mat.at(0, 1) = 1;
    mat.at(0, 2) = 0;
    mat.at(1, 0) = 1;
    mat.at(1, 1) = 1;
    mat.at(1, 2) = 0;

    std::vector<char> vec1 = {1, 0};
    std::vector<char> vec2 = {1, 1, 0};

    std::cout << "mat * vec2:" << std::endl;
    std::vector<char> result1 = mat.multiplyMatrixByVector(vec2);
    for (size_t i = 0; i < result1.size(); ++i) {
        std::cout << (int) result1[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "vec1 * mat:" << std::endl;
    std::vector<char> result2 = mat.multiplyVectorByMatrix(vec1);
    for (size_t i = 0; i < result1.size(); ++i) {
        std::cout << (int) result2[i] << " ";
    }
    std::cout << std::endl;

    matrix::Matrix mat1(vec2); // матрица 1x3
    mat1.printMatrix();
    matrix::Matrix mat2(vec2, true); // матрица 3x1
    mat2.printMatrix();
    std::string str = "010100\n101010\n000011";
    matrix::Matrix matrix(str);
    std::cout << matrix.rows() << " " << matrix.cols() << std::endl;
    matrix.printMatrix();

    return 0;
}