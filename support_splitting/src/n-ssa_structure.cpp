#include "ssa.h"
#include "test_printers.h"

namespace codes {

typedef std::map<std::string, std::vector<std::vector<size_t>>> equivClassesMap;
typedef std::vector<std::vector<std::vector<size_t>>> equivClassesVecSet;

std::vector<size_t> mergeNums(const std::vector<size_t> &cols, const std::vector<size_t> &v) {
    std::vector<size_t> res(cols);
    for (size_t i = 0; i < v.size(); ++i) {
        bool found = false;
        for (size_t j = 0; j < res.size(); ++j) {
            if (res[j] == v[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            res.push_back(v[i]);
        }
    }
    return res;
}

SSANStructure ssaNStructure(codes::Lincode c,
                            std::function<std::string(const codes::Lincode &,
                                                      const std::vector<size_t> &)>
                            invariant,
                            size_t n_sign,
                            std::function<void(codes::Lincode &)> preprocCode) {
    preprocCode(c);
    size_t len = c.len();
    SSANStructure result;
    equivClassesMap equivClasses;
    std::vector<std::vector<size_t>> colCombs = algorithms::generatePermSequences(len, n_sign);
    for (size_t i = 0; i < colCombs.size(); ++i) {
        std::vector<size_t> punctCoords(colCombs[i]);
        std::string invar = invariant(c, colCombs[i]);
        if (equivClasses.find(invar) == equivClasses.end()) {
            equivClasses[invar] = std::vector<std::vector<size_t>>(1);
            equivClasses[invar][0] = colCombs[i];
        } else {
            equivClasses[invar].push_back(colCombs[i]);
        }
        result[colCombs[i]].push_back(std::make_pair(colCombs[i], invar));
    }
    std::vector<std::vector<size_t>> refinders;
    equivClassesVecSet equivClassesVec;
    for (auto iter = equivClasses.begin(); iter != equivClasses.end(); ++iter) {
        if (iter->second.size() == 1) {
            refinders.push_back(iter->second[0]);
        } else {
            equivClassesVec.push_back(iter->second);
        }
    }
    //Before this step we can construct 2-, 3- or 4- signature
    for (size_t refInd = 0; equivClassesVec.size() != 0 && refInd != refinders.size(); ++refInd) {
        std::vector<size_t> invarColumns(refinders[refInd]);
        equivClassesVecSet newEquivClassesVec;
        for (size_t ind = 0; ind != equivClassesVec.size(); ++ind) {
            equivClasses.clear();
            for (size_t i = 0; i < equivClassesVec[ind].size(); ++i) {
                std::vector<size_t> punctCols = mergeNums(invarColumns, equivClassesVec[ind][i]);
                std::string invar = invariant(c, punctCols);
                if (equivClasses.find(invar) == equivClasses.end()) {
                    equivClasses[invar] = std::vector<std::vector<size_t>>(1);
                    equivClasses[invar][0] = equivClassesVec[ind][i];
                } else {
                    equivClasses[invar].push_back(equivClassesVec[ind][i]);
                }
            }
            for (auto iter = equivClasses.begin(); iter != equivClasses.end(); ++iter) {
                if (iter->second.size() == 1) {
                    std::vector<size_t> punctCols = mergeNums(invarColumns, iter->second[0]);
                    refinders.push_back(iter->second[0]);
                    result[iter->second[0]].push_back(std::make_pair(punctCols, iter->first));
                } else {
                    newEquivClassesVec.push_back(iter->second);
                }
            }
        }
        equivClassesVec = newEquivClassesVec;
    }
    for (size_t ind = 0; ind != equivClassesVec.size(); ++ind) {
        for (size_t i = 0; i < equivClassesVec[ind].size(); ++i) {
            std::vector<size_t> col = equivClassesVec[ind][i];
            result[equivClassesVec[ind][i]].push_back(std::make_pair(col, "Not found"));
        }
    }
    return result;
}

void printSSANStructure(const SSANStructure &s) {
    for (auto iter = s.begin(); iter != s.end(); ++iter) {
        std::cout << "[";
        for (size_t j = 0; j != iter->first.size(); ++j) {
            std::cout << iter->first[j] << ", ";
        }
        std::cout << "](" << iter->second.size() << "): ";
        for (size_t j = 0; j != iter->second.size(); ++j) {
            std::cout << " {";
            for (size_t k = 0; k != iter->second[j].first.size(); ++k) {
                std::cout << iter->second[j].first[k] << ", ";
            }
            std::cout << "}:" << iter->second[j].second;
        }
        std::cout << std::endl;
    }
}

} //namespace codes
