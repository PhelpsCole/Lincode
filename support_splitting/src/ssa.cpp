#include "support_splitting.h"
#include "ssa_supports.h"


namespace codes {
using namespace codes::support_ssa;
using namespace codes::invariants;

// Init vector of classes with updating answer
std::vector<SSAData> initEquivClassesVector(const std::vector<std::string> &spectPunct1,
                                            const std::vector<std::string> &spectPunct2,
                                            size_t len, std::vector<size_t> &ans) {
    // Creates equivalation classes {invariant : <set, set>}
    std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> equiv_classes;
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
    std::vector<SSAData> equiv_classes_vec;
    // Parse classes to vector to start n-terative algorithm
    for (auto const &elem: equiv_classes) {
        if (elem.second.first.size() != elem.second.second.size()) {
            // Classes with different sizes 
            //ans = std::vector<size_t>(0);
            return equiv_classes_vec;
        } else {
            std::vector<size_t> v1(elem.second.first.begin(), elem.second.first.end());
            std::vector<size_t> v2(elem.second.second.begin(), elem.second.second.end());
            if (v1.size() == 1) {
                ans[v1[0]] = v2[0] + 1;
            } else {
                SSAData new_data;
                new_data.dif1 = v1;
                new_data.used1 = {};
                new_data.dif2 = v2;
                new_data.used2 = {};
                equiv_classes_vec.push_back(new_data);
            }
        }
    }
    return equiv_classes_vec;
}

// Finds candidate for parsing
AnsParamSet candidateFinder(const std::map<std::string, ComplEqClData> &complexEquivClasses,
                            const std::vector<size_t> &ans, bool &found,
                            const std::vector<size_t> &used1, const std::vector<size_t> &used2) {
    AnsParamSet newCandidateAns;
    newCandidateAns.ans = ans;
    newCandidateAns.cntFound = 0;
    newCandidateAns.newEquivClasses = std::vector<SSAData>();
    for (auto const &elem: complexEquivClasses) {
        std::vector<size_t> v1(elem.second.dif_set1.begin(), elem.second.dif_set1.end());
        std::vector<size_t> v2(elem.second.dif_set2.begin(), elem.second.dif_set2.end());
        std::vector<size_t> copy_ans(ans);
        if (v1.size() != v2.size()) {
            // Bad candidate, |difs1| != |difs2|
            found = false;
            break;
        } else if (v1.size() == 1) {
            newCandidateAns.ans[v1[0]] = v2[0] + 1;
            ++newCandidateAns.cntFound;
        } else {
            SSAData new_data;
            new_data.dif1 = v1;
            new_data.used1 = {elem.second.new_used1};
            new_data.used1.insert(new_data.used1.end(), used1.begin(), used1.end());
            new_data.dif2 = v2;
            new_data.used2 = {elem.second.new_used2};
            new_data.used2.insert(new_data.used2.end(), used2.begin(), used2.end());
            newCandidateAns.newEquivClasses.push_back(new_data);
        }
    }
    return newCandidateAns;
}

//Finds best candidate and say if it's empty
AnsParamSet foundBestCandidate(bool &candidateEmpty, const std::vector<size_t> &ans,
                               const std::map<size_t, std::vector<SpectVectData>> &spectPunctVV1,
                               const std::map<size_t, std::vector<SpectVectData>> &spectPunctVV2,
                               const SSAData &equiv_class) {
    bool found_minimal = false;
    AnsParamSet candidateAns;
    // Iterates by sets of new punctured codes
    for (auto iter = spectPunctVV1.begin(); !found_minimal && iter != spectPunctVV1.end(); ++iter) {
        for (auto iter2 = spectPunctVV2.begin(); !found_minimal && iter2 != spectPunctVV2.end(); ++iter2) {
            std::map<std::string, ComplEqClData> complexEquivClasses;
            bool found;
            // Creates new equiv classes
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
                // Finds candidate and updates found
                AnsParamSet newCandidateAns = candidateFinder(complexEquivClasses, ans, found,
                                                              equiv_class.used1, equiv_class.used2);
                if (found) {
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
    return candidateAns;
}

// Returns vector v(i) = j
std::vector<size_t> support_splitting(const codes::Lincode &c1, const codes::Lincode &c2,
                                      std::function<std::string(const codes::Lincode &)>
                                      invariant) {
    if (c1.len() != c2.len()) {
        // Lenghts of code should be the same
        return std::vector<size_t>(0);
    }
    size_t len = c1.len();
    std::vector<std::string> spectPunct1(len);
    std::vector<std::string> spectPunct2(len);
    std::vector<size_t> columns(1);
    // Create invariants of punctured codes
    for (size_t i = 0; i < len; ++i) {
        columns[0] = i;
        codes::Lincode punct = c1.punctured(columns);
        spectPunct1[i] = invariant(punct);
        punct = c2.punctured(columns);
        spectPunct2[i] = invariant(punct);
    }
    std::vector<size_t> ans(len);
    std::vector<SSAData> equiv_classes_vec = initEquivClassesVector(spectPunct1, spectPunct2, len, ans);
    if (ans.size() == 0) {
        // Parsed classes sizes not equal
        //return std::vector<size_t>(0);
        return ans;
    }
    // Iterates by eq classes while they will not ends
    for (size_t p = 0; p < equiv_classes_vec.size(); ++p) {
        size_t set_size = equiv_classes_vec[p].dif1.size();
        std::map<size_t, std::vector<SpectVectData>> spectPunctVV1;
        std::map<size_t, std::vector<SpectVectData>> spectPunctVV2;
        // Iterates by cols to find not punctured
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
        bool candidateEmpty = true;
        AnsParamSet candidateAns = foundBestCandidate(candidateEmpty, ans,
                                                      spectPunctVV1, spectPunctVV2,
                                                      equiv_classes_vec[p]);
        if (candidateEmpty) {
            //return std::vector<size_t>(0);
            return ans;
        }
        ans = candidateAns.ans;
        if (candidateAns.newEquivClasses.size()) {
            equiv_classes_vec.insert(equiv_classes_vec.end(),
                                     candidateAns.newEquivClasses.begin(),
                                     candidateAns.newEquivClasses.end());
        }
    }
    //if (ansHaveZeroes(ans)) {
    //    return std::vector<size_t>(0);
    //}
    return ans;
}

} //namespace codes