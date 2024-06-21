#include "support_splitting.h"
#include "ssa_test.h"
#include <fstream>
#include <chrono>
#include <ctime>

void testPqsigRM(size_t r, size_t m, size_t invariantId,
                 const std::string &filename, size_t preprocId = 0) {
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode c(pqsigRM);
    codes::SSAStructure s = codes::ssaStructure(c, invariantId, preprocId);
    ssa_test::printSSAStructure(s, filename);
}

void testPqsigRM_N(size_t r, size_t m, size_t invariantId,
                   size_t n_sign, const std::string &filename, size_t preprocId = 0) {
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode c(pqsigRM);
    codes::SSANStructure s = codes::ssaNStructure(c, invariantId, n_sign, preprocId);
    ssa_test::printSSANStructure(s, filename);
}

void testRunner(size_t r, size_t m, size_t invariantId,
                const std::string &filename, size_t nPunct = 1,
                size_t preprocId = 0) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;
    if (nPunct != 1) {
        testPqsigRM_N(r, m, invariantId, nPunct, filename, preprocId);
    } else {
        testPqsigRM(r, m, invariantId, filename, preprocId);
    }
    now = std::chrono::system_clock::now();
    time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Completed computation at " << std::ctime(&time) << std::endl;
    
}

std::string nameFile(size_t r, size_t m, std::string invarName,
                     std::string preprocName, size_t nPunct, size_t cicleStep) {
    std::string res = "modRM_(" + std::to_string(r) + "," + std::to_string(m) + ")_" + invarName;
    if (preprocName != codes::invariants::returnPreprocNameById(0)) {
        res += "_" + preprocName;
    }
    if (cicleStep != 1) {
        res += std::to_string(cicleStep);
    }
    if (nPunct != 1) {
        res += "_" + std::to_string(nPunct);
    }
    res += ".txt";
    return res;
}

int main(int argc, char *argv[]) {
    size_t r = 2;
    size_t m = 7;
    size_t nPunct = 1;
    size_t preprocId = 0;
    size_t invarId = 0;
    size_t cicleIter = 1;
    if (argc == 1) {
        std::cout << "Input args in format: r m nPunct preprocId invarId cicleIter" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 1: " << r << " " << m << " " << nPunct << " " << preprocId << " invarId" << " " << cicleIter << std::endl;
        std::cout << "By 2: " << r << " " << m << " " << nPunct << " preprocId invarId " << cicleIter << std::endl;
        std::cout << "By 3: " << r << " " << m << " nPunct preprocId invarId " << cicleIter << std::endl;
        std::cout << "By 4: " << r << " m nPunct preprocId invarId " << cicleIter << std::endl;
        std::cout << "By 5: r m nPunct preprocId invarId " << cicleIter << std::endl;
        std::cout << "Where preprocId is:" << std::endl;
        std::string zeroReturn = codes::invariants::returnPreprocNameById(preprocId);
        std::string current(zeroReturn);
        do {
            std::cout << preprocId << ": " << current << std::endl;
            current = codes::invariants::returnPreprocNameById(++preprocId);
        } while (current != zeroReturn);
        std::cout << "Where invarId is:" << std::endl;
        zeroReturn = codes::invariants::returnInvarNameById(invarId);
        current = zeroReturn;
        do {
            std::cout << invarId << ": " << current << std::endl;
            current = codes::invariants::returnInvarNameById(++invarId);
        } while (current != zeroReturn);
        return 0;
    } else if (argc == 2) {
        invarId = std::stoi(argv[1]);
    } else if (argc == 3) {
        preprocId = std::stoi(argv[1]);
        invarId = std::stoi(argv[2]);
    } else if (argc == 4) {
        nPunct = std::stoi(argv[1]);
        preprocId = std::stoi(argv[2]);
        invarId = std::stoi(argv[3]);
    } else if (argc == 5) {
        m = std::stoi(argv[1]);
        nPunct = std::stoi(argv[2]);
        preprocId = std::stoi(argv[3]);
        invarId = std::stoi(argv[4]);
    } else if (argc == 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        nPunct = std::stoi(argv[3]);
        preprocId = std::stoi(argv[4]);
        invarId = std::stoi(argv[5]);
    } else if (argc >= 7) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        nPunct = std::stoi(argv[3]);
        preprocId = std::stoi(argv[4]);
        invarId = std::stoi(argv[5]);
        cicleIter = std::stoi(argv[6]);
    }
    for (size_t i = 0; i < cicleIter; ++i) {
        std::string filename = nameFile(r, m, codes::invariants::returnInvarNameById(invarId),
                                        codes::invariants::returnPreprocNameById(preprocId),
                                        nPunct, i + 1);
        testRunner(r, m, invarId, filename, nPunct, preprocId);
    }
    return 0;
}