#include "linear_code.h"

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


int main() {
    codes::Lincode code({{1, 0, 0, 0, 1, 0, 1},
                         {0, 1, 0, 0, 1, 1, 1},
                         {0, 0, 1, 0, 1, 1, 0},
                         {0, 0, 0, 1, 0, 1, 1},});
    testPunctCodes(code, 2);
    testPunctCodes(code, 3);
    return 0;
}