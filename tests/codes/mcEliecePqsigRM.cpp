#include "pqsigRM.h"
#include "permutation.h"

int main(void) {
	std::random_device rd;
    std::mt19937 gen(rd());
    size_t r, m;
	std::cout << "Input generator matrix parameters of your pqsigRM code:" << std::endl;
    std::cout << "r: ";
    std::cin >> r;
    std::cout << "m: ";
    std::cin >> m;
    matrix::Matrix G = codes::pqsigRMGenerator(r, m);
    size_t n = G.cols();
    size_t k = G.rows();

    matrix::Matrix M = matrix::generateRandomNonSingular(k, k);

    std::uniform_int_distribution<int> distrib(1, n);
    matrix::Matrix P = permutation::generateRandomPermutationOfP(n, distrib(gen)).matrix();

    std::cout << "M: " << std::endl;
    M.printMatrix();
    std::cout << "G: " << std::endl;
    G.printMatrix();
    std::cout << "P: " << std::endl;
    P.printMatrix();
    std::cout << "M * G * P: " << std::endl;
    matrix::Matrix res = M * G * P;
    res.printMatrix();

	return 0;
}