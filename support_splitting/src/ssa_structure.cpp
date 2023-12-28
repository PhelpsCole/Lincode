#include "ssa.h"

namespace codes {

typedef std::map<std::string, std::vector<unsigned long long>> equivClassesMap;
typedef std::vector<std::vector<unsigned long long>> equivClassesVecSet;

SSAStructure ssaStructure(codes::Lincode c,
                          size_t invarId,
                          size_t preprocId) {
    codes::invariants::runPreproc(c, preprocId);
    size_t len = c.len();
    std::vector<unsigned long long> columns(1);
    SSAStructure result(len);
    equivClassesMap equivClasses;
    for (size_t i = 0; i < len; ++i) {
        columns[0] = i;
        std::string invar = codes::invariants::runInvariant(c, columns, invarId);
        if (equivClasses.find(invar) == equivClasses.end()) {
            equivClasses[invar] = std::vector<unsigned long long>(1);
            equivClasses[invar][0] = i;
        } else {
            equivClasses[invar].push_back(i);
        }
        result[i].push_back(std::make_pair(columns, invar));
    }
    std::vector<size_t> refinders;
    equivClassesVecSet equivClassesVec;
    for (auto iter = equivClasses.begin(); iter != equivClasses.end(); ++iter) {
        if (iter->second.size() == 1) {
            refinders.push_back(iter->second[0]);
        } else {
            equivClassesVec.push_back(iter->second);
        }
    }
    columns.resize(2);
    for (size_t refInd = 0; equivClassesVec.size() != 0 && refInd != refinders.size(); ++refInd) {
        columns[1] = refinders[refInd];
        equivClassesVecSet newEquivClassesVec;
        for (size_t ind = 0; ind != equivClassesVec.size(); ++ind) {
            equivClasses.clear();
            for (size_t i = 0; i < equivClassesVec[ind].size(); ++i) {
                columns[0] = equivClassesVec[ind][i];
                //std::vector<size_t> punctCoords(columns);
                std::string invar = codes::invariants::runInvariant(c, columns, invarId);
                if (equivClasses.find(invar) == equivClasses.end()) {
                    equivClasses[invar] = std::vector<unsigned long long>(1);
                    equivClasses[invar][0] = equivClassesVec[ind][i];
                } else {
                    equivClasses[invar].push_back(equivClassesVec[ind][i]);
                }
                //result[i].push_back(std::make_pair(punctCoords, invar));
            }
            for (auto iter = equivClasses.begin(); iter != equivClasses.end(); ++iter) {
                if (iter->second.size() == 1) {
                    refinders.push_back(iter->second[0]);
                    columns[0] = iter->second[0];
                    result[iter->second[0]].push_back(std::make_pair(columns, iter->first));
                } else {
                    newEquivClassesVec.push_back(iter->second);
                }
            }
        }
        equivClassesVec = newEquivClassesVec;
    }
    return result;
}

} //namespace codes
