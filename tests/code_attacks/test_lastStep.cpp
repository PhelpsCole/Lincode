#include "codes.h"
#include "matrix.h"
#include "permutation.h"
#include "algorithms.h"
#include <chrono>
#include <ctime>

matrix::Matrix generateRowPerm(unsigned long long size1, unsigned long long size2) {
    matrix::Matrix A(matrix::diag(size1, 1));
    matrix::Matrix B(matrix::generateRandomNonSingular(size1, size2));
    A.concatenateByRows(B);
    matrix::Matrix C(size2, size1);
    B = matrix::diag(size2, 1);
    C.concatenateByRows(B);
    A.concatenateByColumns(C);
    return A;
}

void decodeSearch(std::vector<char> vec, const matrix::Matrix &B) {
    //std::vector<char> c = B.decode(vec);
    //if (c) {
        // нашли
    //}
    for (unsigned long long i = 0; i < vec.size(); ++i) {
        vec[i] ^= 1;
        //c = B.decode(vec);
        //if (c) {
            // нашли
        //    break;
        //}
        vec[i] ^= 1;
    }
}

void wordFinder(const matrix::Matrix &A, unsigned long long minWeight) {
                //const matrix::Matrix &B) {
    for (size_t i = 0; i < A.rows(); ++i) {
        std::vector<char> row(A.row(i));
        auto middle = row.begin() + row.size() / 2;
        std::vector<char> firstPart(row.begin(), middle);
        std::vector<char> secondPart(middle, row.end());
        //std::cout << algorithms::hammingWeight(firstPart) << " ";
        if (algorithms::hammingWeight(firstPart) == minWeight) {
            //std::cout << "(";
            unsigned long long j = 0;
            /*
            for (; j < firstPart.size(); ++j) {
                std::cout << (int) firstPart[j];// << ", ";
            }
            //std::cout << (int) firstPart[j] << "), (";
            std::cout << " ";
            j = 0;
            */
            for (; j < secondPart.size(); ++j) {
                std::cout << (int) secondPart[j] ;//<< ", ";
            }
            //std::cout << (int) secondPart[j] << ")";
            std::cout /*<< " " << algorithms::hammingWeight(secondPart)*/ << std::endl;
            //decodeSearch(secondPart, );
        }
    }
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    if (argc < 3) {
        std::cout << "Input args in format: r m" << std::endl;
        return 0;
    } else if (argc >= 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    }
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Started generating keys at " << std::ctime(&time) << std::endl;
    }
    matrix::Matrix A = codes::RMCode(r, m).toMatrix();
    unsigned long long k1 = A.rows();
    matrix::Matrix P = permutation::generateRandomPermutation(A.cols()).matrix();
    A.concatenateByRows(A * P);
    matrix::Matrix B = codes::RMCode(r - 1, m).toMatrix();
    unsigned long long k2 = B.rows();
    matrix::Matrix C(B.rows(), B.cols());
    C.concatenateByRows(B);
    A.concatenateByColumns(C);

    matrix::Matrix M(generateRowPerm(k1, k2));
    A = M * A;
    A.gaussElimination();
    //A.printVisualMatrix(2);

    wordFinder(A, 1 << (m - r));
    return 0;
}