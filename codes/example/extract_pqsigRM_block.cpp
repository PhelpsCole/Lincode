#include "codes.h"

// Problem with Cmake linkage
void testCodeFromFile(const std::string &filename, char c = ' ', char r = '\n') {
    std::cout << "STARTS testCodeFromFile" << std::endl;
    std::cout << filename << std::endl;
    matrix::Matrix matr = matrix::matrFromFile(filename, c, r);
    codes::Lincode code(matr);
    std::cout << "pqsigRM code sizes:" << std::endl;
    code.printCodeSizes();
    //code.printVisualCode(4);
    std::vector<size_t> rmSizes = codes::rmSizes(code);
    std::cout << "pqsigRM parameters: r = " << rmSizes[0] << ", m = " << rmSizes[1] << std::endl; 
    std::vector<unsigned long long> rows(codes::codeSizeFromRM(rmSizes[0], rmSizes[1] - 2));
    std::vector<unsigned long long> cols(1 << (rmSizes[1] - 2));
    for (size_t i = 0; i < rows.size(); ++i) {
        rows[i] = i;
    }
    for (size_t i = 0; i < cols.size(); ++i) {
        cols[i] = i;
    }
    codes::Lincode subCode(matr.submatrix(rows, cols));
    std::cout << "pqsigRM (1, 1) block:" << std::endl;
    subCode.printVisualCode();
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Input full path to your file with code" << std::endl;
        return 0;
    }
    testCodeFromFile(argv[1]);
    return 0;
}