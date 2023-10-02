#include "pqsigRM.h"

namespace codes {

matrix::Matrix generatePqsigRMBasis(size_t r, size_t m) {
	if (r < 2 || m < 3) {
		throw std::invalid_argument("Bad parameters of pqsigRM");
	}
	matrix::Matrix A = codes::RMCode(r, m - 2).toMatrix();
	matrix::Matrix B = codes::RMCode(r - 1, m - 2).toMatrix();
	matrix::Matrix C = codes::RMCode(r - 2, m - 2).toMatrix();
	size_t size = A.cols();
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<char> distrib(1, size);
    size_t p1 = distrib(gen);
    size_t p2 = distrib(gen);
	A *= matrix::generateRandomPermutation(size, p1);
	C *= matrix::generateRandomPermutation(size, p2);
	matrix::Matrix part1 = A;
	part1.concatenateByRows(A);
	part1.concatenateByRows(part1);

	matrix::Matrix part2 = matrix::Matrix(B.rows(), B.cols());
	part2.concatenateByRows(B);
	part2.concatenateByRows(part2);

	matrix::Matrix part3_1 = matrix::Matrix(B.rows(), 2 * B.cols());
	matrix::Matrix part3_2 = B;
	part3_2.concatenateByRows(B);
	part3_1.concatenateByRows(part3_2);

	matrix::Matrix part4 = matrix::Matrix(C.rows(), 3 * C.cols());
	part4.concatenateByRows(C);

	part1.concatenateByColumns(part2);
	part1.concatenateByColumns(part3_1);
	part1.concatenateByColumns(part4);

	return part1;
}

} //namespace codes