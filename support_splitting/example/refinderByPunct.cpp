#include "support_splitting.h"
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

bool check_signature(const codes::SSAStructure &s, size_t m) {
    std::map<std::string, size_t> counter;
    for (size_t i = 0; i != s.size(); ++i) {
        //In pqsigRM no refinders, so s[i].size() == 1
        for (size_t j = 0; j != s[i].size(); ++j) {
            std::string tmp = s[i][j].second;
            if (counter.find(tmp) == counter.end()) {
                counter[tmp] = 1;
            } else {
                ++counter[tmp];
            }
        }
    }
    std::cout << "Counter:" << std::endl;
    std::vector<size_t> sizes;
    for (auto iter = counter.begin(); iter != counter.end(); ++iter) {
        std::cout << iter->first << ": " << iter->second << std::endl;
        sizes.push_back(iter->second);
    }
    size_t len = 1 << (m - 2);
    if (sizes.size() == 2 && (sizes[0] == len || sizes[1] == len)) {
        return true;
    }
    return false;
}

bool check_signature(const codes::SSANStructure &s, size_t m) {
    std::map<std::string, size_t> counter;
    for (auto iter = s.begin(); iter != s.end(); ++iter) {
        for (size_t j = 0; j != iter->second.size(); ++j) {
            std::string tmp = iter->second[j].second;
            if (counter.find(tmp) == counter.end()) {
                counter[tmp] = 1;
            } else {
                ++counter[tmp];
            }
        }
    }
    std::cout << "Counter:" << std::endl;
    std::vector<size_t> sizes;
    for (auto iter = counter.begin(); iter != counter.end(); ++iter) {
        std::cout << iter->first << ": " << iter->second << std::endl;
        sizes.push_back(iter->second);
    }
    //size_t len = 1 << (m - 2);
    //if (sizes.size() == 2 && (sizes[0] == len || sizes[1] == len)) {
    //    return true;
    //}
    return false;
}

bool testPqsigRM(const codes::Lincode &pqsigRMCode, size_t m, size_t invariantId,
                 const std::string &filename, size_t preprocId = 0) {
    codes::SSAStructure s = codes::ssaStructure(pqsigRMCode, invariantId, preprocId);
    printSSAStructure(s, filename);
    return check_signature(s, m);
}

bool testPqsigRM_N(const codes::Lincode &pqsigRMCode, size_t m, size_t invariantId,
                   size_t n_sign, const std::string &filename, size_t preprocId = 0) {
    codes::SSANStructure s = codes::ssaNStructure(pqsigRMCode, invariantId, n_sign, preprocId);
    printSSANStructure(s, filename);
    return check_signature(s, m);
}

bool testRunner(const codes::Lincode &pqsigRMCode, size_t m, size_t invariantId,
                const std::string &filename, size_t nPunct = 1,
                size_t preprocId = 0) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;
    bool result = false;
    if (nPunct != 1) {
        result = testPqsigRM_N(pqsigRMCode, m, invariantId, nPunct, filename, preprocId);
    } else {
        result = testPqsigRM(pqsigRMCode, m, invariantId, filename, preprocId);
    }
    now = std::chrono::system_clock::now();
    time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Completed computation at " << std::ctime(&time) << std::endl;
    std::cout << std::endl;
    return result;
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
    size_t preprocId = 0;
    size_t invarId = 0;
    size_t cicleIter = 1;
    if (argc == 1) {
        std::cout << "Input args in format: r m preprocId invarId cicleIter" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 1: " << r << " " << m << " " << preprocId << " invarId" << " " << cicleIter << std::endl;
        std::cout << "By 2: " << r << " " << m << " preprocId invarId " << cicleIter << std::endl;
        std::cout << "By 3: " << r << " m preprocId invarId " << cicleIter << std::endl;
        std::cout << "By 4: r m preprocId invarId " << cicleIter << std::endl;
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
        m = std::stoi(argv[1]);
        preprocId = std::stoi(argv[2]);
        invarId = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        preprocId = std::stoi(argv[3]);
        invarId = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        preprocId = std::stoi(argv[3]);
        invarId = std::stoi(argv[4]);
        cicleIter = std::stoi(argv[5]);
    }
    for (size_t i = 0; i < cicleIter; ++i) {
        matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
        codes::Lincode pqsigRMCode(pqsigRM);
        std::cout << "pqsigRM code of sizes r = " << r << "and m = " << m << std::endl;
        pqsigRMCode.printVisualCode(4);
        std::string filename = nameFile(r, m, codes::invariants::returnInvarNameById(invarId),
                                        codes::invariants::returnPreprocNameById(preprocId),
                                        1, i + 1);
        bool flag = testRunner(pqsigRMCode, m, invarId, filename, 1, preprocId);
        if (!flag) {
            for (size_t j = 2; j < 5; ++j) {
                std::string filename = nameFile(r, m, codes::invariants::returnInvarNameById(invarId),
                                                codes::invariants::returnPreprocNameById(preprocId),
                                                j, i + 1);
                testRunner(pqsigRMCode, m, invarId, filename, j, preprocId);
            }
        }
    }
    return 0;
}