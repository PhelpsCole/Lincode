#include "test_printers.h"

namespace codes {
namespace test_printers {
using namespace codes::support_ssa;

void printVV(const std::vector<std::vector<size_t>> &vv) {
    for (size_t i = 0; i < vv.size(); ++i) {
        //std::cout << vv[i].size() << std::endl;
        std::cout << "[" << i << "] ";
        printV<size_t>(vv[i]);
    }
}

void printVPV(const std::vector<std::pair<std::vector<size_t>, std::vector<size_t>>> &v) {
    for (size_t j = 0; j < v.size(); ++j) {
        std::cout << j + 1 << " sets:" << std::endl;
        std::cout << "[1] ";
        printV(v[j].first);
        std::cout << "[2] ";
        printV(v[j].second);
    }
    std::cout << std::endl;
}

void printMSVD(const std::map<size_t, std::vector<SpectVectData>> &map,
               std::vector<size_t> used) {
    std::cout << "printMSVD" << std::endl;
    for (auto iter = map.begin(); iter != map.end(); ++iter) {
        //std::cout << vv[i].size() << std::endl;
        for (size_t j = 0; j < iter->second.size(); ++j) {
            std::cout << "[" << iter->first << ", " << iter->second[j].dif << "]: ";
            std::cout << "spectr: {" << iter->second[j].spectr << "}, used_columns: ";
            printV(used);
        }
        std::cout << std::endl;
    }
}

void printCC(const std::vector<codes::Lincode> &cc) {
    for (size_t i = 0; i < cc.size(); ++i) {
        cc[i].printCode();
    }
}

void printD(const std::map<std::string, std::vector<std::pair<size_t, size_t>>> &d) {
    for (auto iter = d.begin(); iter != d.end(); ++iter) {
        std::cout << "[" << iter->first << "]: ";
        for (size_t i = 0; i < iter->second.size(); ++i) {
            std::cout << "(" << iter->second[i].first << ", " << iter->second[i].second << ") ";
        }
        std::cout << std::endl;
    }
}

void printDD(const std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> &d) {
    for (auto iter = d.begin(); iter != d.end(); ++iter) {
        std::cout << "[" << iter->first << "]: {";
        for (auto iter2 = iter->second.first.begin();
             iter2 != iter->second.first.end(); ++iter2) {
            std::cout << *iter2 << " ";
        }
        std::cout << "}, {";
        for (auto iter2 = iter->second.second.begin();
             iter2 != iter->second.second.end(); ++iter2) {
            std::cout << *iter2 << " ";
        }
        std::cout << "}" << std::endl;
    }
}

void printVSSAData(const std::vector<SSAData> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << i << "'s data of SSData:" << std::endl;
        std::cout << "Differences of 1: ";
        printV(v[i].dif1);
        std::cout << "Differences of 2: ";
        printV(v[i].dif2);
        std::cout << "Used of 1: ";
        printV(v[i].used1);
        std::cout << "Used of 2: ";
        printV(v[i].used2);
        std::cout << std::endl;
    }
}

void printMCECD(const std::map<std::string, ComplEqClData> &d) {
    std::cout << "printMCECD" << std::endl;
    for (auto iter = d.begin(); iter != d.end(); ++iter) {
        std::cout << "[" << iter->first << "]: dif_set1: {";
        for (auto iter2 = iter->second.dif_set1.begin();
             iter2 != iter->second.dif_set1.end(); ++iter2) {
            std::cout << *iter2 << " ";
        }
        std::cout << "}, new_used1: [" << iter->second.new_used1 << "], dif_set2: {";
        for (auto iter2 = iter->second.dif_set2.begin();
             iter2 != iter->second.dif_set2.end(); ++iter2) {
            std::cout << *iter2 << " ";
        }
        std::cout << "}, new_used2: [" << iter->second.new_used2 << "]" << std::endl;
    }
}

void printAnsCandidate(const AnsParamSet &ans) {
    std::cout << "Ans vector: ";
    printV(ans.ans);
    std::cout << "Found new coords: " << ans.cntFound << std::endl;
    std::cout << "Vector of datas:" << std::endl;
    printVSSAData(ans.newEquivClasses);
}

} //namespace test_printers
} //namespace codes