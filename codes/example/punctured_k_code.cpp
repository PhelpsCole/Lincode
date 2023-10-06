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

void testSSA(const codes::Lincode &c1, const codes::Lincode &c2) {
    std::cout << "Inputed codes:" << std::endl;
    c1.printCode();
    c2.printCode();
    std::cout << std::endl;
    std::vector<codes::Lincode> punct_once1(c1.len());
    std::vector<std::vector<size_t>> spectPunct1(c1.len());
    std::vector<codes::Lincode> punct_once2(c2.len());
    std::vector<std::vector<size_t>> spectPunct2(c2.len());
    std::vector<size_t> columns(1);
    for (size_t i = 0; i < punct_once1.size(); ++i) {
        columns[0] = i;
        punct_once1[i] = c1.punctured(columns);
        spectPunct1[i] = punct_once1[i].spectrum_basis();
        punct_once2[i] = c2.punctured(columns);
        spectPunct2[i] = punct_once2[i].spectrum_basis();
    }
    printCC(punct_once1);
    printCC(punct_once2);
    printVV(spectPunct1);
    printVV(spectPunct2);
}

void SSA(const codes::Lincode &c1, const codes::Lincode &c2) {
    std::map<std::string, std::vector<std::pair<size_t, size_t>>> equiv_classes;
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
        spectPunct1[i] = invariant_weight(punct);
        punct = c2.punctured(columns);
        spectPunct2[i] = invariant_weight(punct);
    }
    for (size_t i = 0; i < spectPunct1.size(); ++i) {
        for (size_t j = 0; j < spectPunct2.size(); ++j) {
            if (spectPunct1[i] == spectPunct2[j]) {
                if (equiv_classes.find(spectPunct1[i]) == equiv_classes.end()) {
                    equiv_classes[spectPunct1[i]] = std::vector<std::pair<size_t, size_t>>();
                }
                equiv_classes[spectPunct1[i]].push_back(std::make_pair(i, j));
            }
        }
    }
    printV<std::string>(spectPunct1);
    std::cout << std::endl;
    printV<std::string>(spectPunct2);
    printD(equiv_classes);
}

void SSAOnHull(const codes::Lincode &c1, const codes::Lincode &c2) {
    std::cout << "Inputed codes:" << std::endl;
    c1.printCode();
    c2.printCode();
    std::cout << std::endl;
    std::vector<size_t> dimsPunct1(c1.len());
    std::vector<size_t> dimsPunct2(c2.len());
    std::vector<size_t> columns(1);
    for (size_t i = 0; i < c1.len(); ++i) {
        columns[0] = i;
        codes::Lincode punct = c1.punctured(columns);
        punct = punct.hull();
        punct.printCode();
        dimsPunct1[i] = punct.size();
        punct = c2.punctured(columns);
        punct = punct.hull();
        punct.printCode();
        dimsPunct2[i] = punct.size();
    }
    printV<size_t>(dimsPunct1);
    std::cout << std::endl;
    printV<size_t>(dimsPunct1);
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
    //testSSA(code2, code3);
    //testSSA(code4, code5);
    //SSA(code2, code3);
    //SSAOnHull(code4, code5);
    codes::RMCode rm(5, 3);
    codes::RMCode rm2(5, 3);
    codes::Lincode rm_code(rm.getBasis());
    codes::Lincode rm_code2(rm2.getBasis());
    //SSAOnHull(rm_code, rm_code2);
    SSA(rm_code, rm_code2);
}