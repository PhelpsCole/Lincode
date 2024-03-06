#include "code_attacks.h"

std::vector<matrix::Matrix>
generateCodes(size_t blocksNum, unsigned long long maxCodeSize, unsigned long long maxCodeLen,
              unsigned long long minCodeSize, unsigned long long minCodeLen) {
    std::vector<matrix::Matrix> ans;
    std::uniform_int_distribution<unsigned long long> distribCodeSize(minCodeSize, maxCodeSize);
    std::uniform_int_distribution<unsigned long long> distribCodeLen(minCodeLen, maxCodeLen);
    std::random_device rd;
    std::mt19937 gen(rd());
    for (size_t i = 0; i < blocksNum; ++i) {
        matrix::Matrix newElem(matrix::generateRandomNonSingular(distribCodeSize(gen),
                                                                 distribCodeLen(gen)));
        newElem.gaussElimination();
        ans.push_back(newElem);
    }
    return ans;
}

std::vector<matrix::Matrix>
generateRMCodes(size_t blocksNum, unsigned long long maxR, unsigned long long maxM,
              unsigned long long minR, unsigned long long minM) {
    std::vector<matrix::Matrix> ans;
    std::uniform_int_distribution<unsigned long long> distribR(minR, maxR);
    std::uniform_int_distribution<unsigned long long> distribM(minM, maxM);
    std::random_device rd;
    std::mt19937 gen(rd());
    for (size_t i = 0; i < blocksNum; ++i) {
        matrix::Matrix newElem(codes::RMCode(distribR(gen), distribM(gen)).toMatrix());
        newElem.gaussElimination();
        ans.push_back(newElem);
    }
    return ans;
}

void printCode(const codes::Lincode &concatenatedCode, bool printCodes) { 
    if (printCodes) {
        concatenatedCode.printVisualCode();
    } else {
        concatenatedCode.printCodeSizes();
    }
}

void printMatrix(const matrix::Matrix &matrix, bool printCodes) { 
    if (printCodes) {
        matrix.printVisualMatrix();
    } else {
        std::cout << matrix.rows() << " " << matrix.cols() << std::endl;
    }
}

void testRunner(const codes::Lincode &concatenatedCode, bool printCodes) {
    printCode(concatenatedCode, printCodes);
    std::uniform_int_distribution<int> distrib(1, concatenatedCode.len());
    std::random_device rd;
    std::mt19937 gen(rd());
    matrix::Matrix P = matrix::generateRandomPermutation(concatenatedCode.len(), distrib(gen));
    matrix::Matrix M = matrix::generateRandomNonSingular(concatenatedCode.size(), concatenatedCode.size());

    matrix::Matrix concatenatedMatrix(concatenatedCode.toMatrix());
    //concatenatedMatrix.gaussElimination();
    //printCode(concatenatedMatrix, printCodes);
    concatenatedMatrix = M * concatenatedMatrix * P;
    //concatenatedMatrix *= P;
    std::cout << "START separateConcatenatedCodes" << std::endl;
    P = codes::separateConcatenatedCodes(concatenatedMatrix);
    std::cout << "END separateConcatenatedCodes" << std::endl;
    //printMatrix(P, printCodes);
    concatenatedMatrix *= P;
    concatenatedMatrix.gaussElimination();
    printCode(concatenatedMatrix, printCodes);
}

int main(int argc, char *argv[]) {
    size_t blocksNum = 2;
    bool printCodes = false;
    unsigned long long maxCodeSize = 10;
    unsigned long long minCodeSize = 1;
    unsigned long long maxCodeLen = 10;
    unsigned long long minCodeLen = 1;
    if (argc < 2 || argc == 4 || argc == 6) {
        std::cout << "Input args in format: blocksNum printCodes maxCodeSize"
                  << "maxCodeLen minCodeSize minCodeLen" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 1: blocksNum " << printCodes << " " << maxCodeSize << " "
                  << maxCodeLen << " " << minCodeSize << " " << minCodeLen << std::endl;
        std::cout << "By 2: blocksNum printCodes " << maxCodeSize << " "
                  << maxCodeLen << " " << minCodeSize << " " << minCodeLen << std::endl;
        std::cout << "By 4: blocksNum printCodes maxCodeSize maxCodeLen "
                  << minCodeSize << " " << minCodeLen << std::endl;
        return 0;
    } else if (argc == 2) {
        blocksNum = std::stoi(argv[1]);
    } else if (argc == 3) {
        blocksNum = std::stoi(argv[1]);
        printCodes = std::stoi(argv[2]);
    } else if (argc == 5) {
        blocksNum = std::stoi(argv[1]);
        printCodes = std::stoi(argv[2]);
        maxCodeSize = std::stoi(argv[3]);
        maxCodeLen = std::stoi(argv[4]);
    } else if (argc >= 7) {
        blocksNum = std::stoi(argv[1]);
        printCodes = std::stoi(argv[2]);
        maxCodeSize = std::stoi(argv[3]);
        maxCodeLen = std::stoi(argv[4]);
        minCodeSize = std::stoi(argv[5]);
        minCodeLen = std::stoi(argv[6]);
    }

    std::vector<matrix::Matrix> codesSet = generateRMCodes(blocksNum,
                                                         maxCodeSize, maxCodeLen,
                                                         minCodeSize, minCodeLen);
    for (size_t i = 0; i < codesSet.size(); ++i) {
        std::cout << codesSet[i].rows() << " " << codesSet[i].cols() << std::endl;
        if (printCodes) {
            codesSet[i].printVisualMatrix();
        }
    }
    testRunner(matrix::blockDiag(codesSet), printCodes);
    return 0;
}