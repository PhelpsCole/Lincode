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

void printSSANStructure(const codes::SSANStructure &s,
                        const std::string &filename) {
    std::ofstream out;
    out.open(filename);
    if (out.is_open()) {
        for (auto iter = s.begin(); iter != s.end(); ++iter) {
            out << "(";
            for (size_t j = 0; j != iter->first.size(); ++j) {
                out << iter->first[j] << ", ";
            }
            out << "): ";
            for (size_t j = 0; j != iter->second.size(); ++j) {
                out << " [";
                for (size_t k = 0; k != iter->second[j].first.size(); ++k) {
                    out << iter->second[j].first[k] << ", ";
                }
                out << "]:" << iter->second[j].second;
            }
            out << std::endl;
        }
    }
    out.close();
}

void testPqsigRM(size_t r, size_t m,
                 std::function<std::string(const codes::Lincode &,
                                           const std::vector<size_t> &)> invariant,
                 const std::string &filename) {
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode c(pqsigRM);
    codes::SSAStructure s = codes::ssaStructure(c, invariant);
    printSSAStructure(s, filename);
}

void testPqsigRM_N(size_t r, size_t m,
                   std::function<std::string(const codes::Lincode &,
                                             const std::vector<size_t> &)> invariant,
                   size_t n_sign,
                   const std::string &filename) {
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode c(pqsigRM);
    codes::SSANStructure s = codes::ssaNStructure(c, invariant, n_sign);
    printSSANStructure(s, filename);
}

void testRunner(size_t r, size_t m,
                std::function<std::string(const codes::Lincode &,
                                          const std::vector<size_t> &)> invariant,
                const std::string &filename,
                size_t nPunct = 1) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;
    if (nPunct != 1) {
        testPqsigRM_N(r, m, invariant, nPunct, filename);
    } else {
        testPqsigRM(r, m, invariant, filename);
    }
    
}

std::string nameFile(size_t r, size_t m, std::string invarName, size_t nPunct, size_t cicleStep) {
    std::string res = "modRM_" + std::to_string(r) + "-" + std::to_string(m) + "_" + invarName;
    if (cicleStep != 1) {
        res += std::to_string(cicleStep);
    }
    if (nPunct != 1) {
        res += "_" + std::to_string(nPunct);
    }
    res += ".txt";
    return res;
}

void helpLog() {
    std::cout << "Input args in format: r m nPunct invarId cicleIter" << std::endl;
    std::cout << "Where invarId is:" << std::endl;
    size_t invarId = 0;
    std::string zeroReturn = codes::invariants::returnInvarNameById(invarId);
    std::string current(zeroReturn);
    do {
        std::cout << invarId << ": " << current << std::endl;
        current = codes::invariants::returnInvarNameById(++invarId);
    } while (current != zeroReturn);
}

int main(int argc, char *argv[]) {
    size_t r = 2;
    size_t m = 7;
    size_t nPunct = 1;
    size_t invarId = 0;
    size_t cicleIter = 1;
    if (argc == 1) {
        helpLog();
        return 0;
    } else if (argc == 2) {
        cicleIter = std::stoi(argv[1]);
    } else if (argc == 3) {
        invarId = std::stoi(argv[1]);
        cicleIter = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        cicleIter = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        invarId = std::stoi(argv[3]);
        cicleIter = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        nPunct = std::stoi(argv[3]);
        invarId = std::stoi(argv[4]);
        cicleIter = std::stoi(argv[5]);
    }
    codes::invariants::invarType invarFunc = codes::invariants::returnInvarById(invarId);
    for (size_t i = 0; i < cicleIter; ++i) {
        std::string filename = nameFile(r, m, codes::invariants::returnInvarNameById(invarId),
                                        nPunct, i + 1);
        testRunner(r, m, invarFunc, filename, nPunct);
    }
    return 0;
}