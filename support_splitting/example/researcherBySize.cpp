#include "support_splitting.h"
#include "test_printers.h"
#include <fstream>
#include <chrono>
#include <ctime>

void printSSAStructure(const codes::SSAStructure &s,
                       const std::string &filename) {
    std::ofstream out;
    out.open(filename);
    if (out.is_open()) {
        for (size_t i = 0; i != s.size(); ++i) {
            out << i << "(" << s[i].size() << "): ";
            for (size_t j = 0; j != s[i].size(); ++j) {
                out << " [";
                for (size_t k = 0; k != s[i][j].first.size(); ++k) {
                    out << s[i][j].first[k] << ", ";
                }
                out << "]:" << s[i][j].second;
            }
            out << std::endl;
        }
    }
    out.close();
}

void testIterative(size_t r, size_t m, size_t invariantId, const std::string &filename,
                   size_t preprocId, size_t cicleStep) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;


    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode pqsigRMcode(pqsigRM);
    std::vector<int> maxRMVector = codes::maxRMVector(r, m - 2);
    for (unsigned long long i = 0; i < maxRMVector.size(); ++i) {
        std::string symb;
        if (maxRMVector[i] == -1) {
            symb = "-1";
            pqsigRMcode.dual();
        } else {
            symb = std::to_string(maxRMVector[i]);
            pqsigRMcode = codes::hadPower(pqsigRMcode, maxRMVector[i]);
        }
        codes::SSAStructure s = codes::ssaStructure(pqsigRMcode, invariantId, preprocId);
        std::string tempFilename = filename + '_' + std::to_string(cicleStep)
                                   + '_' + symb + ".txt";
        printSSAStructure(s, tempFilename);
        now = std::chrono::system_clock::now();
        time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << symb << " at " << std::ctime(&time) << std::endl;
    }
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
    size_t r = 2;
    size_t m = 7;
    size_t preprocId = 0;
    size_t invarId = 2;
    size_t cicleIter = 1;
    if (argc <= 2) {
        std::cout << "Input args in format: r m" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << cicleIter << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc >= 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        cicleIter = std::stoi(argv[3]);
    }
    for(size_t i = 1; i <= cicleIter; ++i) {
        std::string filename = nameFile(r, m, codes::invariants::returnInvarNameById(invarId),
                                        "iterativeMaxRM");
        testIterative(r, m, invarId, filename, preprocId, i);
    }
    return 0;
}