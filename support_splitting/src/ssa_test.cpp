#include "ssa_test.h"

namespace ssa_test {

void printSSAStructure(const codes::SSAStructure &s,
                       const std::string &filename) {
    std::ofstream out;
    out.open(filename);
    if (out.is_open()) {
        for (size_t i = 0; i != s.size(); ++i) {
            out << i << "(" << s[i].size() << "): ";
            for (size_t j = 0; j != s[i].size(); ++j) {
                out << " [";
                for (size_t k = 0; k != s[i][j].first.size(); ++k) {
                    out << s[i][j].first[k] << ", ";
                }
                out << "]:" << s[i][j].second;
            }
            out << std::endl;
        }
    }
    out.close();
}

void printSSANStructure(const codes::SSANStructure &s,
                        const std::string &filename) {
    std::ofstream out;
    out.open(filename);
    if (out.is_open()) {
        for (auto iter = s.begin(); iter != s.end(); ++iter) {
            out << "(";
            for (size_t j = 0; j != iter->first.size(); ++j) {
                out << iter->first[j] << ", ";
            }
            out << "): ";
            for (size_t j = 0; j != iter->second.size(); ++j) {
                out << " [";
                for (size_t k = 0; k != iter->second[j].first.size(); ++k) {
                    out << iter->second[j].first[k] << ", ";
                }
                out << "]:" << iter->second[j].second;
            }
            out << std::endl;
        }
    }
    out.close();
}

bool check_signature(const codes::SSAStructure &s, size_t m) {
    std::map<std::string, size_t> counter;
    for (size_t i = 0; i != s.size(); ++i) {
        //In pqsigRM no refinders, so s[i].size() == 1
        for (size_t j = 0; j != s[i].size(); ++j) {
            std::string tmp = s[i][j].second;
            if (counter.find(tmp) == counter.end()) {
                counter[tmp] = 1;
            } else {
                ++counter[tmp];
            }
        }
    }
    //std::cout << "Counter:" << std::endl;
    std::vector<size_t> sizes;
    for (auto iter = counter.begin(); iter != counter.end(); ++iter) {
        //std::cout << iter->first << ": " << iter->second << std::endl;
        sizes.push_back(iter->second);
    }
    // Separated to 2^(m -2) and 2^m - 2^(m -2)
    size_t len = 1 << (m - 2);
    if (sizes.size() == 2 && (sizes[0] == len || sizes[1] == len)) {
        return true;
    }
    return false;
}

bool checkSubblockSignature(const codes::SSAStructure &s, size_t m) {
    std::map<std::string, size_t> counter;
    for (size_t i = 0; i != s.size(); ++i) {
        //In pqsigRM no refinders, so s[i].size() == 1
        for (size_t j = 0; j != s[i].size(); ++j) {
            std::string tmp = s[i][j].second;
            if (counter.find(tmp) == counter.end()) {
                counter[tmp] = 1;
            } else {
                ++counter[tmp];
            }
        }
    }
    std::vector<size_t> sizes;
    for (auto iter = counter.begin(); iter != counter.end(); ++iter) {
        //std::cout << iter->first << ": " << iter->second << std::endl;
        sizes.push_back(iter->second);
    }
    size_t len = 1 << m;
    if (sizes.size() == 2 && (sizes[0] == len || sizes[1] == len)) {
        return true;
    }
    return false;
}

// Need to fix
bool check_signature(const codes::SSANStructure &s, size_t m) {
    std::map<std::string, size_t> counter;
    for (auto iter = s.begin(); iter != s.end(); ++iter) {
        for (size_t j = 0; j != iter->second.size(); ++j) {
            std::string tmp = iter->second[j].second;
            if (counter.find(tmp) == counter.end()) {
                counter[tmp] = 1;
            } else {
                ++counter[tmp];
            }
        }
    }
    std::cout << "Counter:" << std::endl;
    std::vector<size_t> sizes;
    for (auto iter = counter.begin(); iter != counter.end(); ++iter) {
        std::cout << iter->first << ": " << iter->second << std::endl;
        sizes.push_back(iter->second);
    }
    //size_t len = 1 << (m - 2);
    //if (sizes.size() == 2 && (sizes[0] == len || sizes[1] == len)) {
    //    return true;
    //}
    return false;
}

} //namespace ssa_test