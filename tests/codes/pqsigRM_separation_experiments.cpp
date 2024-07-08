#include "codes.h"
#include "encoder.h"

unsigned long long counter(const matrix::Matrix &M, size_t m) {
    std::vector<char> vals(M.cols() / 2);
    for (size_t j = M.cols() / 2; j < M.cols(); ++j) {
        unsigned long long val = 0;
        unsigned long long tmp = 1;
        for (size_t i = 1; i < m + 1; ++i) {
            if (M.at(i,j)) {
                val += tmp;
            }
            tmp <<= 1;
        }
        //if (vals[val] < 2) {
            ++vals[val];
        //}
    }
    unsigned long long mistakesCnt = 0;
    for (size_t i = 0; i < vals.size(); ++i) {
        if (vals[i] == 1) {
            std::cout << "+";
        } else if (vals[i] == 0) {
            std::cout << "-";
        } else {
            mistakesCnt += vals[i] - 1;
            std::cout << int(vals[i]);
        }
    }
    std::cout << std::endl;
    return mistakesCnt;
}

void func(const matrix::Matrix &M1, const matrix::Matrix &M2, size_t m) {
    codes::Encoder enc(M2);
    size_t correctNum = 0;
    std::map<unsigned long long, size_t> mistakeCounter;
    size_t mistakeNum = 0;
    while (!enc.isEnd()) {
        std::vector<char> codeWord(enc.next());
        for (size_t i = 0; i < M1.cols(); ++i) {
            matrix::Matrix copy(M1);
            copy.addRow(i, codeWord);
            //copy.printVisualMatrix(2);
            //std::cout << std::endl;
            unsigned long long mistakes = counter(copy, m);
            if (mistakes == 0) {
                ++correctNum;
            } else {
                ++mistakeNum;
                if (mistakeCounter.find(mistakes) == mistakeCounter.end()) {
                    mistakeCounter[mistakes] = 1;
                } else{
                    ++mistakeCounter[mistakes];
                }
            }
        }
    }
    std::cout << correctNum << " " << mistakeNum << std::endl;
    for (auto elem = mistakeCounter.begin(); elem != mistakeCounter.end(); ++elem) {
        std::cout << "[" << elem->first << "->" << elem->second << "] ";
    }
    std::cout << std::endl;
}


matrix::Matrix generator(size_t r, size_t m, int p = 0) {
    if (r < 2 || m < 3) {
        throw std::invalid_argument("Bad parameters");
    }
    matrix::Matrix A = codes::RMCode(r, m).toMatrix();
    unsigned long long size = A.cols();

    matrix::Matrix B(A);
    if (p == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned long long> distrib(1, size);
        p = distrib(gen);
    } else if (p == -1) {
        p = size;
    }
    B = B * matrix::generateRandomPermutation(size, p);

    matrix::Matrix C = codes::RMCode(r-1, m).toMatrix();

    A.concatenateByRows(B);
    matrix::Matrix part2(matrix::Matrix(C.rows(), C.cols()));
    part2.concatenateByRows(C);
    //func(A, part2, m);
    //return A;
    A.concatenateByColumns(part2);

    // I | M
    // 0 | I
    part2 = matrix::Matrix(C.rows(), B.rows());
    part2.concatenateByRows(matrix::diag(C.rows(), 1));
    matrix::Matrix I = matrix::diag(B.rows(), 1);
    matrix::Matrix M(matrix::generateRandomNonSingular(B.rows(), C.rows()));
    I.concatenateByRows(M);
    I.concatenateByColumns(part2);
    A = I * A;
    std::cout << counter(A, m) << std::endl;
    //matrix::Matrix M = matrix::generateRandomNonSingular(A.rows(), A.rows());
    //A = M * A;
    return A;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    size_t p = 0;
    if (argc < 3) {
        std::cout << "Input args in format: r m p" << std::endl;
        std::cout << "Where p is generation mode" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << p << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        p = std::stoi(argv[3]);
    }
    //generator(r, m, p);
    ///*
    matrix::Matrix G = generator(r, m, p);
    //G.printVisualMatrix(2, true);
    //std::cout << std::endl;
    //G.printMatrix("NoName", 0);
    //*/
	return 0;
}