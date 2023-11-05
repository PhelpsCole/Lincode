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
    matrix::Matrix pqsigRM = codes::pqsigRMMcEliece(r, m);
    codes::Lincode c(pqsigRM);
    codes::SSAStructure s = codes::ssaStructure(c, invariant);
    printSSAStructure(s, filename);
}

void testPqsigRM_N(size_t r, size_t m,
                   std::function<std::string(const codes::Lincode &,
                                             const std::vector<size_t> &)> invariant,
                   size_t n_sign,
                   const std::string &filename) {
    matrix::Matrix pqsigRM = codes::pqsigRMMcEliece(r, m);
    codes::Lincode c(pqsigRM);
    codes::SSANStructure s = codes::ssaNStructure(c, invariant, n_sign);
    printSSANStructure(s, filename);
}

void testRunner(size_t r, size_t m,
                std::function<std::string(const codes::Lincode &,
                                          const std::vector<size_t> &)> invariant,
                const std::string &filename,
                size_t nPunct = 0) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;
    if (nPunct) {
        testPqsigRM_N(r, m, invariant, nPunct, filename);
    } else {
        testPqsigRM(r, m, invariant, filename);
    }
    
}

int main() {
    //Tested
    //testRunner(3, 5, codes::invariants::invariantHullSize, "modRM_3-5_hullSize.txt");
    //testRunner(3, 5, codes::invariants::invariantMinRMSize, "modRM_3-5_minRMSize.txt");
    //testRunner(3, 5, codes::invariants::invariantWeightMinRM, "modRM_3-5_weightMinRM.txt");
    //testRunner(3, 5, codes::invariants::invariantWeightHull, "modRM_3-5_weightHull.txt");

    //Tested
    //testRunner(2, 6, codes::invariants::invariantHullSize, "modRM_2-6_hullSize.txt");
    //testRunner(2, 6, codes::invariants::invariantMinRMSize, "modRM_2-6_minRMSize.txt");
    testRunner(2, 6, codes::invariants::invariantWeightMinRM, "modRM_2-6_weightMinRM2.txt");
    testRunner(3, 6, codes::invariants::invariantWeightMinRM, "modRM_3-6_weightMinRM.txt");
    testRunner(2, 6, codes::invariants::invariantWeightMinRM, "modRM_2-6_weightMinRM3.txt");
    testRunner(3, 6, codes::invariants::invariantWeightMinRM, "modRM_3-6_weightMinRM2.txt");
    testRunner(2, 6, codes::invariants::invariantWeightMinRM, "modRM_2-6_weightMinRM4.txt");
    testRunner(3, 6, codes::invariants::invariantWeightHull, "modRM_3-6_weightHull.txt");
    testRunner(2, 6, codes::invariants::invariantWeightHull, "modRM_2-6_weightHull2.txt");
    testRunner(2, 6, codes::invariants::invariantWeightHull, "modRM_2-6_weightHull3.txt");

    //Tested
    //testRunner(3, 5, codes::invariants::invariantHullSize, "modRM_3-5_hullSize_2.txt", 2);
    //testRunner(3, 5, codes::invariants::invariantMinRMSize, "modRM_3-5_minRMSize_2.txt", 2);
    //testRunner(3, 5, codes::invariants::invariantWeightMinRM, "modRM_3-5_weightMinRM_2.txt", 2);
    //testRunner(3, 5, codes::invariants::invariantWeightHull, "modRM_3-5_weightHull_2.txt", 2);

    //Tested
    //testRunner(3, 5, codes::invariants::invariantHullSize, "modRM_3-5_hullSize_4.txt", 4);
    //testRunner(3, 5, codes::invariants::invariantMinRMSize, "modRM_3-5_minRMSize_4.txt", 4);
    //testRunner(3, 5, codes::invariants::invariantWeightMinRM, "modRM_3-5_weightMinRM_4.txt", 4);
    //testRunner(3, 5, codes::invariants::invariantWeightHull, "modRM_3-5_weightHull_4.txt", 4);

    //Tested
    //testRunner(2, 6, codes::invariants::invariantHullSize, "modRM_2-6_hullSize_2.txt", 2);
    //testRunner(2, 6, codes::invariants::invariantMinRMSize, "modRM_2-6_minRMSize_2.txt", 2);

    //testRunner(3, 8, codes::invariants::invariantMinRMSize, "modRM_3-8_minRMSize.txt");
    //testRunner(3, 8, codes::invariants::invariantMinRMSize, "modRM_3-8_minRMSize_2.txt", 2);
    //testRunner(2, 6, codes::invariants::invariantMinRMSize, "modRM_2-6_minRMSize_4.txt", 4);
    //testRunner(3, 10, codes::invariants::invariantMinRMSize, "modRM_3-10_minRMSize.txt");

    //testRunner(3, 8, codes::invariants::invariantHullSize, "modRM_3-8_hullSize.txt");
    //testRunner(3, 8, codes::invariants::invariantHullSize, "modRM_3-8_hullSize_2.txt", 2);
    //testRunner(2, 6, codes::invariants::invariantHullSize, "modRM_2-6_hullSize_4.txt", 4);
    //testRunner(3, 10, codes::invariants::invariantHullSize, "modRM_3-10_hullSize.txt");
    
    //testRunner(3, 8, codes::invariants::invariantWeightMinRM, "modRM_3-8_weightMinRM.txt");
    //testRunner(2, 6, codes::invariants::invariantWeightMinRM, "modRM_2-6_weightMinRM_2.txt", 2);


    //testRunner(3, 10, codes::invariants::invariantWeightMinRM, "modRM_3-9_weightMinRM.txt");
    //testRunner(3, 8, codes::invariants::invariantWeightMinRM, "modRM_3-8_weightMinRM_2.txt", 2);
    //testRunner(2, 6, codes::invariants::invariantWeightMinRM, "modRM_2-6_weightMinRM_4.txt", 4);



    //testRunner(2, 6, codes::invariants::invariantWeightHull, "modRM_2-6_weightHull_2.txt", 2);
    //testRunner(3, 8, codes::invariants::invariantWeightHull, "modRM_3-8_weightHull.txt");
    //testRunner(3, 8, codes::invariants::invariantWeightHull, "modRM_3-8_weightHull_2.txt", 2);
    //testRunner(2, 6, codes::invariants::invariantWeightHull, "modRM_2-6_weightHull_4.txt", 4);
    //testRunner(3, 10, codes::invariants::invariantWeightHull, "modRM_3-10_weightHull.txt");

    //testRunner(3, 10, codes::invariants::invariantHullSize, "modRM_3-10_hullSize_2.txt", 2);
    //testRunner(3, 10, codes::invariants::invariantMinRMSize, "modRM_3-10_minRMSize_2.txt", 2);
    //testRunner(3, 10, codes::invariants::invariantWeightMinRM, "modRM_3-10_weightMinRM_2.txt", 2);
    //testRunner(3, 10, codes::invariants::invariantWeightHull, "modRM_3-10_weightHull_2.txt", 2);

    //testRunner(4, 10, codes::invariants::invariantHullSize, "modRM_4-10_hullSize.txt");
    //testRunner(4, 10, codes::invariants::invariantMinRMSize, "modRM_4-10_minRMSize.txt");
    //testRunner(4, 10, codes::invariants::invariantWeightMinRM, "modRM_4-10_weightMinRM.txt");
    //testRunner(4, 10, codes::invariants::invariantWeightHull, "modRM_4-10_weightHull.txt");

    //testRunner(3, 8, codes::invariants::invariantHullSize, "modRM_3-7_hullSize_4.txt", 4);
    //testRunner(3, 8, codes::invariants::invariantMinRMSize, "modRM_3-7_minRMSize_4.txt", 4);
    //testRunner(3, 8, codes::invariants::invariantWeightMinRM, "modRM_3-7_weightMinRM_4.txt", 4);
    //testRunner(3, 8, codes::invariants::invariantWeightHull, "modRM_3-7_weightHull_4.txt", 4);

    //testRunner(4, 10, codes::invariants::invariantHullSize, "modRM_4-10_hullSize_2.txt", 2);
    //testRunner(4, 10, codes::invariants::invariantMinRMSize, "modRM_4-10_minRMSize_2.txt", 2);
    //testRunner(4, 10, codes::invariants::invariantWeightMinRM, "modRM_4-10_weightMinRM_2.txt", 2);
    //testRunner(4, 10, codes::invariants::invariantWeightHull, "modRM_4-10_weightHull_2.txt", 2);

    //testRunner(3, 10, codes::invariants::invariantHullSize, "modRM_3-9_hullSize_4.txt", 4);
    //testRunner(3, 10, codes::invariants::invariantMinRMSize, "modRM_3-9_minRMSize_4.txt", 4);
    //testRunner(3, 10, codes::invariants::invariantWeightMinRM, "modRM_3-9_weightMinRM_4.txt", 4);
    //testRunner(3, 10, codes::invariants::invariantWeightHull, "modRM_3-9_weightHull_4.txt", 4);
    return 0;
}