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

void printV(std::vector<size_t> &v) {
    for (size_t j = 0; j < v.size(); ++j) {
        std::cout << v[j] << " ";
    }
    std::cout << std::endl;
}

void printVV(std::vector<std::vector<size_t>> &vv) {
    for (size_t i = 0; i < vv.size(); ++i) {
        //std::cout << vv[i].size() << std::endl;
        std::cout << "[" << i << "] ";
        printV(vv[i]);
    }
}

void printCC(std::vector<codes::Lincode> &cc) {
    for (size_t i = 0; i < cc.size(); ++i) {
        cc[i].printCode();
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
    std::cout << "Inputed codes:" << std::endl;
    c1.printCode();
    c2.printCode();
    std::cout << std::endl;
    std::vector<std::vector<size_t>> spectPunct1(c1.len());
    std::vector<std::vector<size_t>> spectPunct2(c2.len());
    std::vector<size_t> columns(1);
    for (size_t i = 0; i < c1.len(); ++i) {
        columns[0] = i;
        codes::Lincode punct = c1.punctured(columns);
        spectPunct1[i] = punct.spectrum_basis();
        punct = c2.punctured(columns);
        spectPunct2[i] = punct.spectrum_basis();
    }
    printVV(spectPunct1);
    std::cout << std::endl;
    printVV(spectPunct2);
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
    printV(dimsPunct1);
    std::cout << std::endl;
    printV(dimsPunct1);
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