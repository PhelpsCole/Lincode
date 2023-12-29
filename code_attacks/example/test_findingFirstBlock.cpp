#include "code_attacks.h"
#include "support_splitting.h"
#include <fstream>
#include <chrono>
#include <ctime>

void testfindingFirstBlock(size_t r, size_t m, bool testMode) {
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode pqsigRMcode(pqsigRM);
    std::vector<unsigned long long>
    blockColumns(codes::attackSupporters::findingFirstBlock(pqsigRMcode, testMode));
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
    bool testMode = false;
    if (argc <= 2) {
        std::cout << "Input args in format: r m testMode" << std::endl;
        std::cout << "testMode is a flag of printing files and logs" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << testMode << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc >= 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        testMode = std::stoi(argv[3]);
    }
    testfindingFirstBlock(r, m, testMode);
    return 0;
}