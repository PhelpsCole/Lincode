#include "support_splitting.h"
#include "test_printers.h"

namespace codes {

// Returns vector v(i) = j
std::vector<size_t> support_splitting(const codes::Lincode &c1, const codes::Lincode &c2,
                                      std::function<std::string(const codes::Lincode &)>
                                      invariant) {
    using namespace codes::support_ssa;
    using namespace codes::invariants;
    using namespace codes::test_printers;
    if (c1.len() != c2.len()) {
        return std::vector<size_t>(0);
    }
    size_t len = c1.len();
    std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> equiv_classes;
    std::cout << "Inputed codes:" << std::endl;
    c1.printCode();
    c2.printCode();
    std::cout << std::endl;
    std::vector<std::string> spectPunct1(len);
    std::vector<std::string> spectPunct2(len);
    std::vector<size_t> columns(1);
    // Creates invariants of punctured codes
    for (size_t i = 0; i < len; ++i) {
        columns[0] = i;
        codes::Lincode punct = c1.punctured(columns);
        spectPunct1[i] = invariant(punct);
        punct = c2.punctured(columns);
        spectPunct2[i] = invariant(punct);
    }
    printV(spectPunct1);
    std::cout << std::endl;
    printV(spectPunct2);
    // Creates equivalation classes
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < len; ++j) {
            if (spectPunct1[i] == spectPunct2[j]) {
                if (equiv_classes.find(spectPunct1[i]) == equiv_classes.end()) {
                    std::set<size_t> s1{i};
                    std::set<size_t> s2{j};
                    equiv_classes[spectPunct1[i]] = std::make_pair(s1, s2);
                } else {
                    equiv_classes[spectPunct1[i]].first.insert(i);
                    equiv_classes[spectPunct1[i]].second.insert(j);
                }
            }
        }
    }
    printDD(equiv_classes);
    //parse classes to vector to start n-terative algorithm
    std::vector<size_t> ans(len);
    std::vector<SSAData> equiv_classes_vec;
    for (auto const &elem: equiv_classes) {
        if (elem.second.first.size() != elem.second.second.size()) {
            return std::vector<size_t>(0);
        } else {
            std::vector<size_t> v1(elem.second.first.begin(), elem.second.first.end());
            std::vector<size_t> v2(elem.second.second.begin(), elem.second.second.end());
            if (v1.size() == 1) {
                ans[v1[0]] = v2[0] + 1;
            } else {
                sort(v1.begin(), v1.end());
                sort(v2.begin(), v2.end());
                SSAData new_data;
                new_data.dif1 = v1;
                new_data.used1 = {};
                new_data.dif2 = v2;
                new_data.used2 = {};
                equiv_classes_vec.push_back(new_data);
            }
        }
    }
    printVSSAData(equiv_classes_vec);
    // Iterates till vector ends
    //Iterates by eq classes while they will not ends
    for (size_t p = 0; p < equiv_classes_vec.size(); ++p) {
        size_t set_size = equiv_classes_vec[p].dif1.size();
        std::map<size_t, std::vector<SpectVectData>> spectPunctVV1;
        std::map<size_t, std::vector<SpectVectData>> spectPunctVV2;
        //iterates by cols to find not punctured
        for (size_t i = 0; i < len; ++i) {
            if (!inSSDataFirst(equiv_classes_vec[p], i)) {
                spectPunctVV1[i] = spectPunctVector(c1, equiv_classes_vec[p].used1,
                                                    i, equiv_classes_vec[p].dif1,
                                                    set_size, invariant);
            }
            if (!inSSDataSecond(equiv_classes_vec[p], i)) {
                spectPunctVV2[i] = spectPunctVector(c2, equiv_classes_vec[p].used2,
                                                    i, equiv_classes_vec[p].dif2,
                                                    set_size, invariant);
            }
        }
        printMSVD(spectPunctVV1, equiv_classes_vec[p].used1);
        std::cout << std::endl;
        printMSVD(spectPunctVV2, equiv_classes_vec[p].used2);
        bool found_minimal = false;
        AnsParamSet candidateAns;
        bool candidateEmpty = true;
        for (auto iter = spectPunctVV1.begin(); !found_minimal && iter != spectPunctVV1.end(); ++iter) {
            AnsParamSet newCandidateAns;
            newCandidateAns.ans = ans;
            newCandidateAns.cntFound = 0;
            newCandidateAns.newEquivClasses = std::vector<SSAData>();
            for (auto iter2 = spectPunctVV2.begin(); !found_minimal && iter2 != spectPunctVV2.end(); ++iter2) {
                std::map<std::string, ComplEqClData> complexEquivClasses;
                bool found;
                for (size_t i = 0; i < iter->second.size(); ++i) {
                    found = false;
                    for (size_t j = 0; j < iter2->second.size(); ++j) {
                        if (iter->second[i].spectr == iter2->second[j].spectr) {
                            found = true;
                            if (complexEquivClasses.find(iter->second[i].spectr)
                                == complexEquivClasses.end()) {
                                ComplEqClData tmp;
                                std::set<size_t> tmpSet{iter->second[i].dif};
                                tmp.dif_set1 = tmpSet;
                                std::set<size_t> tmpSet2{iter2->second[j].dif};
                                tmp.dif_set2 = tmpSet2;
                                tmp.new_used1 = iter->first;
                                tmp.new_used2 = iter2->first;
                                complexEquivClasses[iter->second[i].spectr] = tmp;
                            } else {
                                complexEquivClasses[iter->second[i].spectr].dif_set1.insert(iter->second[i].dif);
                                complexEquivClasses[iter2->second[i].spectr].dif_set2.insert(iter2->second[j].dif);
                            }
                        }
                    }
                    if (!found) {
                        break;
                    }
                }
                if (found) {
                    for (auto const &elem: complexEquivClasses) {
                        std::vector<size_t> v1(elem.second.dif_set1.begin(), elem.second.dif_set1.end());
                        std::vector<size_t> v2(elem.second.dif_set2.begin(), elem.second.dif_set2.end());
                        std::vector<size_t> copy_ans(ans);
                        if (v1.size() != v2.size()) {
                            //bad candidate, |difs1| != |difs2|
                            found = false;
                            break;
                        } else if (v1.size() == 1) {
                            newCandidateAns.ans[v1[0]] = v2[0] + 1;
                            ++newCandidateAns.cntFound;
                        } else {
                            sort(v1.begin(), v1.end());
                            sort(v2.begin(), v2.end());
                            SSAData new_data;
                            new_data.dif1 = v1;
                            new_data.used1 = {elem.second.new_used1};
                            new_data.used1.insert(new_data.used1.end(),
                                                  equiv_classes_vec[p].used1.begin(),
                                                  equiv_classes_vec[p].used1.end());
                            new_data.dif2 = v2;
                            new_data.used2 = {elem.second.new_used2};
                            new_data.used2.insert(new_data.used2.end(),
                                                  equiv_classes_vec[p].used2.begin(),
                                                  equiv_classes_vec[p].used2.end());
                            newCandidateAns.newEquivClasses.push_back(new_data);
                        }
                    }
                    if (found) {
                        std::cout << iter->first << " " << iter2->first << std::endl;
                        printMCECD(complexEquivClasses);
                        std::cout << std::endl;
                    }
                    if (found) {
                        std::cout << "Prev candidate:" << std::endl;
                        printAnsCandidate(candidateAns);
                        std::cout << "New candidate:" << std::endl;
                        printAnsCandidate(newCandidateAns);
                        if (newCandidateAns.newEquivClasses.size() == 0) {
                            candidateAns = newCandidateAns;
                            found_minimal = true;
                            candidateEmpty = false;
                        } else if (candidateEmpty ||
                                   isBetterCandidate(newCandidateAns, candidateAns)) {
                            candidateAns = newCandidateAns;
                            candidateEmpty = false;
                        }
                    }
                }
                complexEquivClasses.clear();
            }
        }
        printV(ans);
        if (candidateEmpty) {
            return std::vector<size_t>(0);
        }
        ans = candidateAns.ans;
        if (candidateAns.newEquivClasses.size()) {
            equiv_classes_vec.insert(equiv_classes_vec.end(),
                                     candidateAns.newEquivClasses.begin(),
                                     candidateAns.newEquivClasses.end());
        }
        printV(ans);
    }
    if (ansHaveZeroes(ans)) {
        return std::vector<size_t>(0);
    }
    return ans;
}

} //namespace codes