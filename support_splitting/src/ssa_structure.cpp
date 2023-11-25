#include "ssa.h"
#include "test_printers.h"

//
#include <chrono>
#include <ctime>
//

namespace codes {

typedef std::map<std::string, std::vector<size_t>> equivClassesMap;
typedef std::vector<std::vector<size_t>> equivClassesVecSet;

SSAStructure ssaStructure(codes::Lincode c,
                          size_t invarId,
                          size_t preprocId) {
    codes::invariants::runPreproc(c, preprocId);
    /**/
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Completed preprocCode at " << std::ctime(&time) << std::endl;
    /**/
    size_t len = c.len();
    std::vector<size_t> columns(1);
    SSAStructure result(len);
    equivClassesMap equivClasses;
    for (size_t i = 0; i < len; ++i) {
        columns[0] = i;
        std::string invar = codes::invariants::runInvariant(c, columns, invarId);
        if (equivClasses.find(invar) == equivClasses.end()) {
            equivClasses[invar] = std::vector<size_t>(1);
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
                    equivClasses[invar] = std::vector<size_t>(1);
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
    //for (size_t ind = 0; ind != equivClassesVec.size(); ++ind) {
    //    for (size_t i = 0; i < equivClassesVec[ind].size(); ++i) {
    //        std::vector<size_t> col = {equivClassesVec[ind][i]};
    //        result[equivClassesVec[ind][i]].push_back(std::make_pair(col, "Not found"));
    //    }
    //}
    return result;
}

void printSSAStructure(const SSAStructure &s) {
    for (size_t i = 0; i != s.size(); ++i) {
        std::cout << i << "(" << s[i].size() << "): ";
        for (size_t j = 0; j != s[i].size(); ++j) {
            std::cout << " [";
            for (size_t k = 0; k != s[i][j].first.size(); ++k) {
                std::cout << s[i][j].first[k] << ", ";
            }
            std::cout << "]:" << s[i][j].second;
        }
        std::cout << std::endl;
    }
}

} //namespace codes
