#include "code_attacks.h"
#include "support_splitting.h"
#include <fstream>
#include <chrono>
#include <ctime>

void testfindingFirstBlock(size_t r, size_t m, size_t sigma, bool returnMax, bool testMode) {
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode pqsigRMcode(pqsigRM);
    std::vector<unsigned long long> blockColumns;
    if (sigma == 1) {
        blockColumns
            = codes::attackSupporters::findingBlockColumns(pqsigRMcode, r, m, returnMax, testMode);
    } else {
        size_t rowsStart(codes::codeSizeFromRM(r, m - 2));
        std::vector<unsigned long long> rows(codes::codeSizeFromRM(r, m) - rowsStart);
        std::vector<unsigned long long> cols(3 * (1 << (m - 2)));
        std::cout << rows.size() << " " << cols.size() << std::endl;
        std::iota(cols.begin(), cols.end(), 1 << (m - 2));
        std::iota(rows.begin(), rows.end(), rowsStart);
        matrix::Matrix pqsigRMSubcode(pqsigRMcode.toMatrix().submatrix(rows, cols));
        blockColumns
            = codes::attackSupporters::findingBlockColumns(codes::Lincode(pqsigRMSubcode),
                                                           r - 1, m, returnMax, testMode);
    }
    if (blockColumns.size() != 0) {
        std::cout << "First block is in columns:" << std::endl;
        for (size_t i = 0; i < blockColumns.size(); ++i) {
            std::cout << blockColumns[i] << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Block wasn't found" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    size_t r = 2;
    size_t m = 7;
    size_t sigma = 1;
    bool returnMax = false;
    bool testMode = false;
    if (argc <= 2) {
        std::cout << "Input args in format: r m sigma returnMax testMode" << std::endl;
        std::cout << "testMode is a flag of printing files and logs" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << sigma << " " << testMode << std::endl;
        std::cout << "By 3: r m sigma" << testMode << std::endl;
        std::cout << "By 4: r m sigma returnMax" << testMode << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        sigma = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        sigma = std::stoi(argv[3]);
        returnMax = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        sigma = std::stoi(argv[3]);
        returnMax = std::stoi(argv[4]);
        testMode = std::stoi(argv[5]);
    }
    testfindingFirstBlock(r, m, sigma, returnMax, testMode);
    return 0;
}