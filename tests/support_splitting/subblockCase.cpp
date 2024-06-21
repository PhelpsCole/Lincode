#include "support_splitting.h"
#include "ssa_test.h"
#include <fstream>
#include <chrono>
#include <ctime>

enum { PREPROC_SIMPLE_ID = 0 };

void testIterative(size_t r, size_t m, size_t invariantId,
                   const std::string &filename, size_t cicleStep) {
    size_t preprocId = PREPROC_SIMPLE_ID;

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;


    matrix::Matrix pqsigRMSubblock = codes::pqsigRMSubblockGenerator(r, m);
    codes::Lincode subblock(pqsigRMSubblock);
    subblock.dual();
    r = m - r;
    codes::Lincode startCode(subblock);
    std::vector<int> maxRMVector = codes::maxRMVector(r, m);
    if (maxRMVector.size() == 0) {
        throw std::invalid_argument("Wrong r and m parameters.");
    }
    std::string symb;
    codes::SSAStructure s;
    for (unsigned long long i = 0; i < maxRMVector.size(); ++i) {
        if (maxRMVector[i] == -1) {
            symb += "-1|";
            subblock.dual();
            //pqsigRMcode = pqsigRMcode.hull();
        } else {
            symb += std::to_string(maxRMVector[i]) + "|";
            subblock = codes::hadPower(subblock, maxRMVector[i]);

            s = codes::ssaStructure(subblock, invariantId, preprocId);

            now = std::chrono::system_clock::now();
            time = std::chrono::system_clock::to_time_t(now);
            std::cout << "Completed computation of " << symb << " at " << std::ctime(&time) << std::endl;

            if (ssa_test::checkSubblockSignature(s, m)) {
                std::string tempFilename = filename + '_' + symb  
                                           + '_' + std::to_string(cicleStep) + "_found.txt";
                ssa_test::printSSAStructure(s, tempFilename);
                return;
            }
        }
    }
    std::string tempFilename = filename + '_' + symb  
                               + '_' + std::to_string(cicleStep) + ".txt";
    std::string unFoundMatrix = filename + '_' + symb  
                               + "_matrix" + std::to_string(cicleStep) + ".txt";
    ssa_test::printSSAStructure(s, tempFilename);
    startCode.printCode(unFoundMatrix);
}

std::string nameFile(size_t r, size_t m, std::string invarName,
                     std::string preprocName) {
    std::string res = "modRM_(" + std::to_string(r) + "," + std::to_string(m) + ")_" + invarName;
    if (preprocName != codes::invariants::returnPreprocNameById(0)) {
        res += "_" + preprocName;
    }
    return res;
}

int main(int argc, char *argv[]) {
    size_t r = 3;
    size_t m = 5;
    size_t invarId = 2;
    size_t cicleIter = 1;
    if (argc <= 2) {
        std::cout << "Input args in format: r m invarId cicleIter" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << invarId << " " << cicleIter << std::endl;
        std::cout << "By 3: r m " << invarId << " cicleIter" << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        cicleIter = std::stoi(argv[3]);
    } else if (argc >= 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        invarId = std::stoi(argv[3]);
        cicleIter = std::stoi(argv[4]);
    }
    for(size_t i = 1; i <= cicleIter; ++i) {
        std::string filename = nameFile(r, m, codes::invariants::returnInvarNameById(invarId),
                                        "iterativeMaxRM");
        testIterative(r, m, invarId, filename, i);
    }
    return 0;
}