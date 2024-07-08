#include "codes.h"
#include "encoder.h"

bool counter(const matrix::Matrix &M, size_t m) {
    std::vector<char> vals(M.cols() / 2);
    bool found = true;
    for (size_t j = M.cols() / 2; j < M.cols(); ++j) {
        unsigned long long val = 0;
        unsigned long long tmp = 1;
        for (size_t i = 1; i < m + 1; ++i) {
            if (M.at(i,j)) {
                val += tmp;
            }
            tmp <<= 1;
        }
        ++vals[val];
        if (vals[val] == 2) {
            found = false;
        }
    }
    for (size_t i = 0; i < vals.size(); ++i) {
        if (vals[i] == 1) {
            std::cout << "+";
        } else if (vals[i] == 0) {
            std::cout << "-";
        } else {
            std::cout << int(vals[i]);
        }
    }
    std::cout << std::endl;
    return found;
}

bool attack(const matrix::Matrix &M1, const matrix::Matrix &M2, size_t m) {
    codes::Encoder enc(M2);
    while (!enc.isEnd()) {
        std::vector<char> codeWord(enc.next());
        for (size_t i = 0; i < M1.cols(); ++i) {
            matrix::Matrix copy(M1);
            copy.addRow(i, codeWord);
            codes::Encoder enc2(M2);
            std::vector<char> codeWordTmp;
            while (!enc2.isEnd()) {
                codeWord = enc2.next();
                for (size_t j = 0; j < M1.cols(); ++j) {
                    if (j != 0) {
                        copy.addRow(j-1, codeWordTmp);
                    }
                    copy.addRow(j, codeWord);
                    if (counter(copy, m)) {
                        std::cout << i << std::endl;
                        codeWord = enc.stage();
                        for (size_t j = 0; j < codeWord.size(); ++j) {
                            if (codeWord[j] == 0) {
                                std::cout << "-";
                            } else {
                                std::cout << "+";
                            }
                        }
                        std::cout << std::endl;
                        copy.printVisualMatrix(2);
                        return true;
                    }
                }
                codeWordTmp = codeWord;
            }
        }
    }
    std::cout << "Not found" << std::endl;
    return false;
}


bool generator(size_t r, size_t m, int p = 0) {
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
    A.concatenateByColumns(part2);

    // I | M
    // 0 | I
    matrix::Matrix OI = matrix::Matrix(C.rows(), B.rows());
    OI.concatenateByRows(matrix::diag(C.rows(), 1));
    matrix::Matrix I = matrix::diag(B.rows(), 1);
    matrix::Matrix M(matrix::generateRandomNonSingular(B.rows(), C.rows()));
    I.concatenateByRows(M);
    I.concatenateByColumns(OI);
    A = I * A;
    return attack(A, part2, m);
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
    size_t cnt = 0;
    while(!generator(r, m, p)) {
        ++cnt;
        std::cout << cnt << std::endl;
    }
	return 0;
}