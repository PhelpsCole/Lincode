#include "ssa.h"


namespace codes {

typedef std::map<std::string, std::vector<std::vector<unsigned long long>>> equivClassesMap;
typedef std::vector<std::vector<std::vector<unsigned long long>>> equivClassesVecSet;

std::vector<unsigned long long> mergeNums(const std::vector<unsigned long long> &cols,
                                          const std::vector<unsigned long long> &v) {
    std::vector<unsigned long long> res(cols);
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
                            size_t invarId,
                            size_t n_sign,
                            size_t preprocId) {
    codes::invariants::runPreproc(c, preprocId);
    size_t len = c.len();
    SSANStructure result;
    equivClassesMap equivClasses;
    std::vector<std::vector<unsigned long long>> colCombs = algorithms::generatePermSequences(len, n_sign);
    for (size_t i = 0; i < colCombs.size(); ++i) {
        std::string invar = codes::invariants::runInvariant(c, colCombs[i], invarId);
        if (equivClasses.find(invar) == equivClasses.end()) {
            equivClasses[invar] = std::vector<std::vector<unsigned long long>>(1);
            equivClasses[invar][0] = colCombs[i];
        } else {
            equivClasses[invar].push_back(colCombs[i]);
        }
        result[colCombs[i]].push_back(std::make_pair(colCombs[i], invar));
    }
    std::vector<std::vector<unsigned long long>> refinders;
    equivClassesVecSet equivClassesVec;
    for (auto iter = equivClasses.begin(); iter != equivClasses.end(); ++iter) {
        if (iter->second.size() == 1) {
            refinders.push_back(iter->second[0]);
        } else {
            equivClassesVec.push_back(iter->second);
        }
    }
    for (size_t refInd = 0; equivClassesVec.size() != 0 && refInd != refinders.size(); ++refInd) {
        std::vector<unsigned long long> invarColumns(refinders[refInd]);
        equivClassesVecSet newEquivClassesVec;
        for (size_t ind = 0; ind != equivClassesVec.size(); ++ind) {
            equivClasses.clear();
            for (size_t i = 0; i < equivClassesVec[ind].size(); ++i) {
                std::vector<unsigned long long> punctCols = mergeNums(invarColumns, equivClassesVec[ind][i]);
                std::string invar = codes::invariants::runInvariant(c, punctCols, invarId);
                if (equivClasses.find(invar) == equivClasses.end()) {
                    equivClasses[invar] = std::vector<std::vector<unsigned long long>>(1);
                    equivClasses[invar][0] = equivClassesVec[ind][i];
                } else {
                    equivClasses[invar].push_back(equivClassesVec[ind][i]);
                }
            }
            for (auto iter = equivClasses.begin(); iter != equivClasses.end(); ++iter) {
                if (iter->second.size() == 1) {
                    std::vector<unsigned long long> punctCols = mergeNums(invarColumns, iter->second[0]);
                    refinders.push_back(iter->second[0]);
                    result[iter->second[0]].push_back(std::make_pair(punctCols, iter->first));
                } else {
                    newEquivClassesVec.push_back(iter->second);
                }
            }
        }
        equivClassesVec = newEquivClassesVec;
    }
    //for (size_t ind = 0; ind != equivClassesVec.size(); ++ind) {
    //    for (size_t i = 0; i < equivClassesVec[ind].size(); ++i) {
    //        std::vector<size_t> col = equivClassesVec[ind][i];
    //        result[equivClassesVec[ind][i]].push_back(std::make_pair(col, "Not found"));
    //    }
    //}
    return result;
}

} //namespace codes
