#include "pqsigRM.h"

namespace codes {

matrix::Matrix pqsigRMGenerator(size_t r, size_t m) {
    if (r < 2 || m < 3) {
        throw std::invalid_argument("Bad parameters of pqsigRM");
    }
    matrix::Matrix A = codes::RMCode(r, m - 2).toMatrix();
    matrix::Matrix B = codes::RMCode(r - 1, m - 2).toMatrix();
    matrix::Matrix C = codes::RMCode(r - 2, m - 2).toMatrix();
    unsigned long long size = A.cols();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long> distrib(1, size);
    unsigned long long p1 = distrib(gen);
    unsigned long long p2 = distrib(gen);
    //std::cout << "p1 = " << p1 << ", p2 = " << p2 << std::endl;
    A *= matrix::generateRandomPermutation(size, p1);
    C *= matrix::generateRandomPermutation(size, p2);
    matrix::Matrix part1 = A;
    part1.concatenateByRows(A);
    part1.concatenateByRows(part1); // A^s|A^s|A^s|A^s

    matrix::Matrix part2 = matrix::Matrix(B.rows(), B.cols());
    part2.concatenateByRows(B);
    part2.concatenateByRows(part2); // 0|B|0|B

    matrix::Matrix part3_1 = matrix::Matrix(B.rows(), 2 * B.cols());
    matrix::Matrix part3_2 = B;
    part3_2.concatenateByRows(B);
    part3_1.concatenateByRows(part3_2); // 0|0|B|B

    matrix::Matrix part4 = matrix::Matrix(C.rows(), 3 * C.cols());
    part4.concatenateByRows(C); // 0|0|0|C^s2

    part1.concatenateByColumns(part2);
    part1.concatenateByColumns(part3_1);
    part1.concatenateByColumns(part4);

    return part1;
}

matrix::Matrix pqsigRMMcEliece(size_t r, size_t m) {
    matrix::Matrix G = pqsigRMGenerator(r, m);
    unsigned long long n = 1 << m;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distrib(1, n);
    matrix::Matrix P = matrix::generateRandomPermutation(n, distrib(gen));
    return matrix::generateRandomNonSingular(G.rows(), G.rows()) * G * P;
}

void maxRMSubMatrPqsigRM(codes::Lincode &pqsigRMcode, bool safe) {
    std::vector<size_t> rmSizes = codes::rmSizes(pqsigRMcode);
    std::vector<int> maxRMVector = codes::maxRMVector(rmSizes[0], rmSizes[1] - 2);
    for (unsigned long long i = 0; i < maxRMVector.size(); ++i) {
        if (maxRMVector[i] == -1) {
            pqsigRMcode.dual();
        } else {
            pqsigRMcode = codes::hadPower(pqsigRMcode, maxRMVector[i], safe);
        }
    }
}
} //namespace codes