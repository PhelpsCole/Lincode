#include "support_splitting.h"
#include "ssa_test.h"
#include <fstream>
#include <chrono>
#include <ctime>

enum { PREPROC_SIMPLE_ID = 0 };

void testIterative(size_t r, size_t m, size_t invariantId,
                   const std::string &filename, size_t cicleStep,
                   size_t nPunct = 1) {
    size_t preprocId = PREPROC_SIMPLE_ID;

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;


    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode pqsigRMcode(pqsigRM);
    pqsigRMcode = pqsigRMcode.hull();
    codes::Lincode startCode(pqsigRMcode);

    //std::vector<int> maxRMVector = codes::maxRMVector(r, m - 2);
    //if (maxRMVector.size() == 0) {
    //    throw std::invalid_argument("Wrong r and m parameters.");
    //}
    std::string symb;
    codes::SSAStructure s;
    codes::SSANStructure s2;
    size_t qMax = (m - 2) / (m - 2 - r - 1);
    for (unsigned long long i = 1; i <= qMax + 1; ++i) {
        symb += std::to_string(i) + "|";
        pqsigRMcode = codes::hadPower(startCode, i);
        if (nPunct == 1) {
            s = codes::ssaStructure(pqsigRMcode, invariantId, preprocId);
        } else {
            s2 = codes::ssaNStructure(pqsigRMcode, invariantId, nPunct, preprocId);
        }

        now = std::chrono::system_clock::now();
        time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << symb << " at " << std::ctime(&time) << std::endl;

        std::string tempFilename = filename + '_' + symb  
                                   + '_' + std::to_string(cicleStep) + "_found.txt";
        if (nPunct == 1) {
            if (ssa_test::check_signature(s, m)) {
                std::string tempFilename = filename + '_' + symb  
                                           + '_' + std::to_string(cicleStep) + "_found.txt";
                ssa_test::printSSAStructure(s, tempFilename);
                return;
            } else {
                std::string tempFilename = filename + '_' + symb  
                                           + '_' + std::to_string(cicleStep) + "_notFound.txt";
                ssa_test::printSSAStructure(s, tempFilename);
            }
        } else {
            ssa_test::printSSANStructure(s2, tempFilename);
        }
    }
    std::string tempFilename = filename + '_' + symb  
                               + '_' + std::to_string(cicleStep) + ".txt";
    std::string unFoundMatrix = filename + '_' + symb  
                               + "_matrix" + std::to_string(cicleStep) + ".txt";
    if (nPunct == 1) {
        ssa_test::printSSAStructure(s, tempFilename);
    } else {
        ssa_test::printSSANStructure(s2, tempFilename);
    }
    startCode.printCode(unFoundMatrix);
}

std::string nameFile(size_t r, size_t m, std::string invarName,
                     std::string preprocName, size_t nPunct) {
    std::string res = "modRM_(" + std::to_string(r) + "," + std::to_string(m) + ")_" + invarName;
    if (preprocName != codes::invariants::returnPreprocNameById(0)) {
        res += "_" + preprocName;
    }
    if (nPunct != 1) {
        res += "_" + std::to_string(nPunct);
    }
    return res;
}

int main(int argc, char *argv[]) {
    size_t r = 2;
    size_t m = 7;
    size_t invarId = 2;
    size_t cicleIter = 1;
    size_t nPunct = 1;
    if (argc <= 2) {
        std::cout << "Input args in format: r m nPunct invarId cicleIter" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << nPunct << " " << invarId << " " << cicleIter << std::endl;
        std::cout << "By 3: r m nPunct " << invarId << " " << cicleIter << std::endl;
        std::cout << "By 4: r m nPunct " << invarId << " cicleIter" << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        nPunct = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        nPunct = std::stoi(argv[3]);
        cicleIter = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        nPunct = std::stoi(argv[3]);
        invarId = std::stoi(argv[4]);
        cicleIter = std::stoi(argv[5]);
    }
    for(size_t i = 1; i <= cicleIter; ++i) {
        std::string filename = nameFile(r, m, codes::invariants::returnInvarNameById(invarId),
                                        "iterativeMaxRM", nPunct);
        testIterative(r, m, invarId, filename, i, nPunct);
    }
    return 0;
}