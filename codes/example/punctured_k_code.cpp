#include "linear_code.h"
#include "rm_code.h"

struct SSAData {
    std::vector<size_t> dif1;
    std::vector<size_t> used1;
    std::vector<size_t> dif2;
    std::vector<size_t> used2;
};

struct SpectVectData {
    std::string spectr;
    std::vector<size_t> used;
    size_t dif;
};

bool inSSDataFirst(const SSAData &d, size_t elem) {
    return std::find(d.dif1.begin(), d.dif1.end(), elem) != d.dif1.end() ||
           (d.used1.size() && std::find(d.used1.begin(), d.used1.end(), elem) != d.used1.end());
}

bool inSSDataSecond(const SSAData &d, size_t elem) {
    return std::find(d.dif2.begin(), d.dif2.end(), elem) != d.dif2.end() ||
           (d.used2.size() && std::find(d.used2.begin(), d.used2.end(), elem) != d.used2.end());
}

std::vector<SpectVectData>
spectPunctVector(const codes::Lincode &c,
                 std::vector<size_t> &used, size_t i,
                 std::vector<size_t> &dif, size_t set_size,
                 std::function<std::string(const codes::Lincode &)> invariant) {
    std::vector<size_t> columns;
    std::vector<SpectVectData> res;
    //iterates by punct codes in eq class
    for (size_t j = 0; j < set_size; ++j) {
        columns.insert(columns.end(), used.begin(), used.end());
        columns.push_back(i);
        columns.push_back(dif[j]);
        codes::Lincode punct = c.punctured(columns);
        SpectVectData new_data;
        new_data.spectr = invariant(punct);
        new_data.used = used;
        new_data.dif = dif[j];
        res.push_back(new_data);
    }
    return res;
}

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
void printV(const std::vector<T> &v) {
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

void printVPV(std::vector<std::pair<std::vector<size_t>, std::vector<size_t>>> &v) {
    for (size_t j = 0; j < v.size(); ++j) {
        std::cout << j + 1 << " sets:" << std::endl;
        std::cout << "[1] ";
        printV(v[j].first);
        std::cout << "[2] ";
        printV(v[j].second);
    }
    std::cout << std::endl;
}

void printMSVD(const std::map<size_t, std::vector<SpectVectData>> &map) {
    std::cout << "printMSVD" << std::endl;
    for (auto iter = map.begin(); iter != map.end(); ++iter) {
        //std::cout << vv[i].size() << std::endl;
        for (size_t j = 0; j < iter->second.size(); ++j) {
            std::cout << "[" << iter->first << ", " << iter->second[j].dif << "]: ";
            std::cout << "spectr: {" << iter->second[j].spectr << "}, used_columns: ";
            printV(iter->second[j].used);
        }
        std::cout << std::endl;
    }
}

void printCC(std::vector<codes::Lincode> &cc) {
    for (size_t i = 0; i < cc.size(); ++i) {
        cc[i].printCode();
    }
}

void printD(const std::map<std::string, std::vector<std::pair<size_t, size_t>>> &d) {
    for (auto iter = d.begin(); iter != d.end(); ++iter) {
        std::cout << "[" << iter->first << "]: ";
        for (size_t i = 0; i < iter->second.size(); ++i) {
            std::cout << "(" << iter->second[i].first << ", " << iter->second[i].second << ") ";
        }
        std::cout << std::endl;
    }
}

void printDD(const std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> &d) {
    for (auto iter = d.begin(); iter != d.end(); ++iter) {
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

void printVSSAData(std::vector<SSAData> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << i << "'s data of SSData:" << std::endl;
        printV(v[i].dif1);
        printV(v[i].dif2);
        printV(v[i].used1);
        printV(v[i].used2);
        std::cout << std::endl;
    }
}

// Returns vector v(i) = j
std::vector<size_t> SSA(const codes::Lincode &c1, const codes::Lincode &c2,
                           std::function<std::string(const codes::Lincode &)> invariant) {
    if (c1.len() != c2.len()) {
        throw std::invalid_argument("Lenghts of code should be the same");
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
    printV<std::string>(spectPunct1);
    std::cout << std::endl;
    printV<std::string>(spectPunct2);
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
    //parse classes and creates recurtion
    std::vector<size_t> ans(len);
    std::vector<SSAData> equiv_classes_vec;
    for (auto const &elem: equiv_classes) {
        if (elem.second.first.size() != elem.second.second.size()) {
            return std::vector<size_t>(0);
        } else {
            std::vector<size_t> v1(elem.second.first.begin(), elem.second.first.end());
            std::vector<size_t> v2(elem.second.second.begin(), elem.second.second.end());
            if (v1.size() == 1) {
                ans[v1[0]] = v2[0];
            } else {
                sort(v1.begin(), v1.end());
                sort(v2.begin(), v2.end());
                SSAData new_data;
                new_data.dif1 = v1;
                new_data.used1 = {};
                new_data.dif2 = v1;
                new_data.used2 = {};
                equiv_classes_vec.push_back(new_data);
            }
        }
    }
    printVSSAData(equiv_classes_vec);
    // Iterates till vector ends
    while (equiv_classes_vec.size() != 0) {
        size_t size = equiv_classes_vec.size();
        //Iterates by eq classes
        for (size_t p = 0; p < size; ++p) {
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
            printMSVD(spectPunctVV1);
            std::cout << std::endl;
            printMSVD(spectPunctVV2);
        }
        break;
    }
    return ans;
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
    codes::RMCode rm(2, 3);
    codes::RMCode rm2(2, 3);
    codes::Lincode rm_code(rm.getBasis());
    codes::Lincode rm_code2(rm2.getBasis());
    //SSA(rm_code, rm_code2, invariant_weight);
    //SSA(rm_code, rm_code2, invariant_size);
    //SSA(code5, code4, invariant_weight);
    SSA(rm_code, rm_code2, invariant_weight);
}