#include "ssa.h"

namespace codes {

// Init vector of classes with updating answer
bool updateTransformStack(SSASingleData &transformStack,
                          SSAStructure &ans,
                          const std::map<size_t, std::pair<std::string, std::vector<size_t>>> &spectPunct) {
    // Creates equivalation classes {invariant : <set, set>}
    std::map<std::string, std::vector<std::pair<size_t, std::vector<size_t>>>> equiv_classes;
    for (auto iter = spectPunct.begin(); iter != spectPunct.end(); ++iter) {
        if (equiv_classes.find(iter->second.first) == equiv_classes.end()) {
            std::set<size_t> s1{iter->first};
            equiv_classes[iter->second.first].push_back(std::make_pair(iter->first,
                                                                       iter->second.second));
        } else {
            equiv_classes[iter->second.first].push_back(std::make_pair(iter->first,
                                                                       iter->second.second));
        }
    }
    // Parse classes to vector to start n-terative algorithm
    for (auto const &elem: equiv_classes) {
        std::vector<std::pair<size_t, std::vector<size_t>>> v1(elem.second.begin(), elem.second.end());
        if (v1.size() == 1) {
            // v(i) = j
            ans[v1[0].first] = std::make_pair(v1[0].second, elem.first);
            transformStack.found.push_back(v1[0].first);
        } else {
            std::vector<size_t> tmp(v1.size());
            for (size_t i = 0; i < v1.size(); ++i) {
                tmp[i] = v1[i].first;
            }
            transformStack.to_find.insert(transformStack.to_find.begin(), tmp.begin(), tmp.end());
        }
    }
    return true;
}

SSAStructure ssaStructure(const codes::Lincode &c,
                               std::function<std::string(const codes::Lincode &,
                                                         const std::vector<size_t> &)>
                               invariant) {
    size_t len = c.len();
    std::map<size_t, std::pair<std::string, std::vector<size_t>>> spectPunct1;
    std::vector<size_t> columns(1);
    for (size_t i = 0; i < len; ++i) {
        columns[0] = i;
        spectPunct1[i] = std::make_pair(invariant(c, columns), columns);
    }
    SSAStructure result;
    SSASingleData transformStack;
    transformStack.found = std::vector<size_t>();
    transformStack.to_find = std::vector<size_t>();
    // updates stack and ans, returns false if cl are not equal
    if (!updateTransformStack(transformStack, result, spectPunct1)) {
        return result;
    }
    // Iterates by eq classes while they will not ends
    for (size_t p = 0; transformStack.to_find.size() != 0 && p < transformStack.found.size(); ++p) {
        std::vector<size_t> pair_cols;
        pair_cols = std::vector<size_t>(2);
        pair_cols[0] = transformStack.found[p];
        spectPunct1.clear();
        for (size_t i = 0; i < transformStack.to_find.size(); ++i) {
            pair_cols[1] = transformStack.to_find[i];
            spectPunct1[transformStack.to_find[i]] = std::make_pair(invariant(c, pair_cols), pair_cols);
        }
        transformStack.to_find.clear();
        if (!updateTransformStack(transformStack, result, spectPunct1)) {
            return result;
        }
    }
    return result;
}

} //namespace codes
