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
    //std::cout << "Counter:" << std::endl;
    std::vector<size_t> sizes;
    for (auto iter = counter.begin(); iter != counter.end(); ++iter) {
        //std::cout << iter->first << ": " << iter->second << std::endl;
        sizes.push_back(iter->second);
    }
    // Separated to 2^(m -2) and 2^m - 2^(m -2)
    size_t len = 1 << (m - 2);
    if (sizes.size() == 2 && (sizes[0] == len || sizes[1] == len)) {
        return true;
    }
    return false;
}

void testIterative(size_t r, size_t m, size_t invariantId,
                   const std::string &filename, size_t cicleStep) {
    size_t preprocId = 0;

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;


    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode pqsigRMcode(pqsigRM);
    std::vector<int> maxRMVector = codes::maxRMVector(r, m - 2);
    if (maxRMVector.size() == 0) {
        throw std::invalid_argument("Wrong r and m parameters.");
    }
    std::string symb;
    codes::SSAStructure s;
    bool safe = false;
    if (m > 11) {
        safe = true;
        symb += "safe";
    }
    for (unsigned long long i = 0; i < maxRMVector.size(); ++i) {
        if (maxRMVector[i] == -1) {
            symb += "-1|";
            pqsigRMcode.dual();
        } else {
            symb += std::to_string(maxRMVector[i]) + "|";
            pqsigRMcode = codes::hadPower(pqsigRMcode, maxRMVector[i], safe);

            s = codes::ssaStructure(pqsigRMcode, invariantId, preprocId);

            now = std::chrono::system_clock::now();
            time = std::chrono::system_clock::to_time_t(now);
            std::cout << "Completed computation of " << symb << " at " << std::ctime(&time) << std::endl;

            if (check_signature(s, m)) {
                std::string tempFilename = filename + '_' + symb  
                                           + '_' + std::to_string(cicleStep) + "_found.txt";
                printSSAStructure(s, tempFilename);
                return;
            }
        }
    }
    std::string tempFilename = filename + '_' + symb  
                               + '_' + std::to_string(cicleStep) + ".txt";
    printSSAStructure(s, tempFilename);
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
    //size_t preprocId = 0;
    size_t invarId = 2;
    size_t cicleIter = 1;
    if (argc <= 2) {
        std::cout << "Input args in format: r m invarId cicleIter" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << invarId << " " << cicleIter << std::endl;
        std::cout << "By 3: r m invarId " << cicleIter << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        invarId = std::stoi(argv[3]);
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