#include "ssa.h"

namespace codes {

// Init vector of classes with updating answer
bool updateTransformStack(SSAData &transformStack, std::vector<size_t> &ans,
                          const std::map<size_t, std::string> &spectPunct1,
                          const std::map<size_t, std::string> &spectPunct2) {
    // Creates equivalation classes {invariant : <set, set>}
    std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> equiv_classes;
    for (auto iter = spectPunct1.begin(); iter != spectPunct1.end(); ++iter) {
        for (auto iter2 = spectPunct2.begin(); iter2 != spectPunct2.end(); ++iter2) {
            if (iter->second == iter2->second) {
                if (equiv_classes.find(iter->second) == equiv_classes.end()) {
                    std::set<size_t> s1{iter->first};
                    std::set<size_t> s2{iter2->first};
                    equiv_classes[iter->second] = std::make_pair(s1, s2);
                } else {
                    equiv_classes[iter->second].first.insert(iter->first);
                    equiv_classes[iter2->second].second.insert(iter2->first);
                }
            }
        }
    }
    std::vector<size_t> tmp(ans);
    std::vector<size_t> tmpToFind1;
    std::vector<size_t> tmpToFind2;
    // Parse classes to vector to start n-terative algorithm
    bool bad_try = false;
    for (auto const &elem: equiv_classes) {
        if (elem.second.first.size() != elem.second.second.size()) {
            // Classes with different sizes
            //return false;
            bad_try = true;
        } else {
            std::vector<size_t> v1(elem.second.first.begin(), elem.second.first.end());
            std::vector<size_t> v2(elem.second.second.begin(), elem.second.second.end());
            if (v1.size() == 1) {
                // v(i) = j
                tmp[v1[0]] = v2[0] + 1;
                transformStack.found1.push_back(v1[0]);
                transformStack.found2.push_back(v2[0]);
            } else {
                tmpToFind1.insert(tmpToFind1.begin(), v1.begin(), v1.end());
                tmpToFind2.insert(tmpToFind2.begin(), v2.begin(), v2.end());
            }
        }
    }
    if (!bad_try) {
        ans = tmp;
        transformStack.to_find1 = tmpToFind1;
        transformStack.to_find2 = tmpToFind2;
    }
    return true;
}

std::vector<size_t> support_splitting(const codes::Lincode &c1, const codes::Lincode &c2,
                                      std::function<std::string(const codes::Lincode &,
                                                                const std::vector<size_t> &)>
                                      invariant) {
    if (c1.len() != c2.len()) {
        // Lenghts of code should be the same
        return std::vector<size_t>(0);
    }
    size_t len = c1.len();
    std::map<size_t, std::string> spectPunct1;
    std::map<size_t, std::string> spectPunct2;
    std::vector<size_t> columns(1);
    // Create invariants of punctured codes
    for (size_t i = 0; i < len; ++i) {
        columns[0] = i;
        spectPunct1[i] = invariant(c1, columns);
        spectPunct2[i] = invariant(c2, columns);
    }
    std::vector<size_t> ans(len);
    SSAData transformStack;
    transformStack.found1 = std::vector<size_t>();
    transformStack.to_find1 = std::vector<size_t>();
    transformStack.found2 = std::vector<size_t>();
    transformStack.to_find2 = std::vector<size_t>();
    // updates stack and ans, returns false if cl are not equal
    if (!updateTransformStack(transformStack, ans, spectPunct1, spectPunct2)) {
        return std::vector<size_t>(0);
    }
    // Iterates by eq classes while they will not ends
    for (size_t p = 0; transformStack.to_find1.size() != 0 && p < transformStack.found1.size(); ++p) {
        std::pair<std::vector<size_t>, std::vector<size_t>> pair_cols;
        pair_cols.first = std::vector<size_t>(2);
        pair_cols.second = std::vector<size_t>(2);
        pair_cols.first[0] = transformStack.found1[p];
        pair_cols.second[0] = transformStack.found2[p];
        spectPunct1.clear();
        spectPunct2.clear();
        for (size_t i = 0; i < transformStack.to_find1.size(); ++i) {
            pair_cols.first[1] = transformStack.to_find1[i];
            pair_cols.second[1] = transformStack.to_find2[i];
            spectPunct1[transformStack.to_find1[i]] = invariant(c1, pair_cols.first);
            spectPunct2[transformStack.to_find2[i]] = invariant(c2, pair_cols.second);
        }
        transformStack.to_find1.clear();
        transformStack.to_find2.clear();
        if (!updateTransformStack(transformStack, ans, spectPunct1, spectPunct2)) {
            return std::vector<size_t>(0);
        }
    }
    return ans;
}

} //namespace codes
