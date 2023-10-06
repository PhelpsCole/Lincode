#include "linear_code.h"
#include "rm_code.h"

// Returns all punctured codes in num columns
std::vector<codes::Lincode> punctCodes(codes::Lincode &code, size_t num) {
    std::vector<codes::Lincode> res;
    std::vector<std::vector<size_t>> seqs = algorithms::generatePermSequences(code.len(), num);
    for (size_t i = 0; i < seqs.size(); ++i) {
        res.push_back(code.punctured(seqs[i]));
    }
    return res;
}

void testPunctCodes(codes::Lincode &code, size_t n) {
    std::cout << "STARTS testPunctCodes" << std::endl;
    std::cout << "Punctured " << n << "times codes:" << std::endl;
    std::vector<codes::Lincode> v = punctCodes(code, n);
    for (size_t i = 0; i < v.size(); ++i) {
        v[i].printCode();
    }
}

std::string invariant_weight(const codes::Lincode &code) {
    std::vector<size_t> spectr = code.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

std::string invariant_size(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    //hull.printCode();
    return std::to_string(hull.size());
}

template<typename T>
void printV(std::vector<T> &v) {
    for (size_t j = 0; j < v.size(); ++j) {
        std::cout << v[j] << " ";
    }
    std::cout << std::endl;
}

void printVV(std::vector<std::vector<size_t>> &vv) {
    for (size_t i = 0; i < vv.size(); ++i) {
        //std::cout << vv[i].size() << std::endl;
        std::cout << "[" << i << "] ";
        printV<size_t>(vv[i]);
    }
}

void printCC(std::vector<codes::Lincode> &cc) {
    for (size_t i = 0; i < cc.size(); ++i) {
        cc[i].printCode();
    }
}

void printD(const std::map<std::string, std::vector<std::pair<size_t, size_t>>> &d) {
    for(auto iter = d.begin(); iter != d.end(); ++iter) {
        std::cout << "[" << iter->first << "]: ";
        for (size_t i = 0; i < iter->second.size(); ++i) {
            std::cout << "(" << iter->second[i].first << ", " << iter->second[i].second << ") ";
        }
        std::cout << std::endl;
    }
}

void printDD(const std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> &d) {
    for(auto iter = d.begin(); iter != d.end(); ++iter) {
        std::cout << "[" << iter->first << "]: {";
        for (auto iter2 = iter->second.first.begin();
             iter2 != iter->second.first.end(); ++iter2) {
            std::cout << *iter2 << " ";
        }
        std::cout << "}, {";
        for (auto iter2 = iter->second.second.begin();
             iter2 != iter->second.second.end(); ++iter2) {
            std::cout << *iter2 << " ";
        }
        std::cout << "}" << std::endl;
    }
}

void SSA(const codes::Lincode &c1, const codes::Lincode &c2,
         std::function<std::string(const codes::Lincode &)> invariant) {
    std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> equiv_classes;
    std::cout << "Inputed codes:" << std::endl;
    c1.printCode();
    c2.printCode();
    std::cout << std::endl;
    std::vector<std::string> spectPunct1(c1.len());
    std::vector<std::string> spectPunct2(c2.len());
    std::vector<size_t> columns(1);
    for (size_t i = 0; i < c1.len(); ++i) {
        columns[0] = i;
        codes::Lincode punct = c1.punctured(columns);
        spectPunct1[i] = invariant(punct);
        punct = c2.punctured(columns);
        spectPunct2[i] = invariant(punct);
    }
    for (size_t i = 0; i < spectPunct1.size(); ++i) {
        for (size_t j = 0; j < spectPunct2.size(); ++j) {
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
    printV<std::string>(spectPunct1);
    std::cout << std::endl;
    printV<std::string>(spectPunct2);
    printDD(equiv_classes);
}

int main() {
    codes::Lincode code({{1, 0, 0, 0, 1, 0, 1},
                         {0, 1, 0, 0, 1, 1, 1},
                         {0, 0, 1, 0, 1, 1, 0},
                         {0, 0, 0, 1, 0, 1, 1},});
    //testPunctCodes(code, 2);
    //testPunctCodes(code, 3);
    codes::Lincode code2({{1, 1, 1, 0},
                         {0, 1, 1, 1},
                         {1, 0, 1, 0},});
    codes::Lincode code3({{0, 0, 1, 1},
                         {1, 0, 1, 1},
                         {1, 1, 0, 1},});
    codes::Lincode code4({{0, 1, 1, 0, 1},
                         {0, 1, 0, 1, 1},
                         {0, 1, 1, 1, 0},
                         {1, 0, 1, 0, 1},
                         {1, 1, 1, 1, 0},});
    codes::Lincode code5({{1, 0, 1, 0, 1},
                          {0, 0, 1, 1, 1},
                          {1, 0, 0, 1, 1},
                          {1, 1, 1, 0, 0},
                          {1, 1, 0, 1, 1},});
    //SSA(code2, code3);
    //SSA(code4, code5);
    codes::RMCode rm(5, 3);
    codes::RMCode rm2(5, 3);
    codes::Lincode rm_code(rm.getBasis());
    codes::Lincode rm_code2(rm2.getBasis());
    SSA(rm_code, rm_code2, invariant_weight);
    //SSA(rm_code, rm_code2, invariant_size);
    SSA(code5, code4, invariant_weight);
}