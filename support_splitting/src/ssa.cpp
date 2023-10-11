#include "ssa.h"

namespace codes {

// Init vector of classes with updating answer
bool updateTransformStack(SSAData &transformStack, std::vector<size_t> &ans, size_t len,
                          const std::vector<std::string> &spectPunct1,
                          const std::vector<std::string> &spectPunct2) {
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
    // Parse classes to vector to start n-terative algorithm
    for (auto const &elem: equiv_classes) {
        if (elem.second.first.size() != elem.second.second.size()) {
            // Classes with different sizes
            return false;
        } else {
            std::vector<size_t> v1(elem.second.first.begin(), elem.second.first.end());
            std::vector<size_t> v2(elem.second.second.begin(), elem.second.second.end());
            if (v1.size() == 1) {
                // v(i) = j
                ans[v1[0]] = v2[0] + 1;
                transformStack.found1.push_back(v1[0]);
                transformStack.found2.push_back(v2[0]);
            } else {
                transformStack.to_find1.insert(transformStack.to_find1.begin(), v1.begin(), v1.end());
                transformStack.to_find2.insert(transformStack.to_find2.begin(), v2.begin(), v2.end());
            }
        }
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
    std::vector<std::string> spectPunct1(len);
    std::vector<std::string> spectPunct2(len);
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
    if (!updateTransformStack(transformStack, ans, len, spectPunct1, spectPunct2)) {
        return std::vector<size_t>(0);
    }
    // Iterates by eq classes while they will not ends
    for (size_t p = 0; p < transformStack.found1.size(); ++p) {
        std::pair<std::vector<size_t>, std::vector<size_t>> pair_cols;
        pair_cols.first = std::vector<size_t>(2);
        pair_cols.second = std::vector<size_t>(2);
        pair_cols.first[0] = transformStack.found1[p];
        pair_cols.second[0] = transformStack.found2[p];
        spectPunct1.resize(transformStack.to_find1.size());
        spectPunct2.resize(transformStack.to_find1.size()); //should be equal sizes
        for (size_t i = 0; i < transformStack.to_find1.size(); ++i) {
            pair_cols.first[1] = i;
            pair_cols.second[1] = i;
            spectPunct1[i] = invariant(c1, pair_cols.first);
            spectPunct2[i] = invariant(c2, pair_cols.second);
        }
        transformStack.to_find1.clear();
        transformStack.to_find2.clear();
        if (!updateTransformStack(transformStack, ans, len, spectPunct1, spectPunct2)) {
            return std::vector<size_t>(0);
        }
    }
    return ans;
}

} //namespace codes
