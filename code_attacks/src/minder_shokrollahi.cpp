#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

bool vInsideVv(const std::vector<std::vector<char>> &vv, const std::vector<char> &v) {
    for (size_t i = 0; i < vv.size(); ++i) {
        if (vv[i].size() != v.size()) {
            std::cerr << vv[i].size() << " " << v.size() << std::endl;
            throw std::invalid_argument("Bad vector in vv");
        }
        bool inside = true;
        for (size_t j = 0; j < vv[i].size(); ++j) {
            if (vv[i][j] != v[j]) {
                inside = false;
                break;
            }
        }
        if (inside) {
            return inside;
        }
    }
    return false;
}

// Minder-Shokrollahi algorithm of reduction RM(r, m) -> RM(r-1,m) !!!
codes::Lincode rmReductor(const codes::Lincode &rm) {
    codes::Lincode B;
    std::vector<size_t> rmSizes = codes::rmSizes(rm);
    unsigned long long size = codeSizeFromRM(rmSizes[0] - 1, rmSizes[1]);
    unsigned long long weight = 1 << (rmSizes[1] - rmSizes[0]);
    std::vector<std::vector<char>> usedCodeWords;
    codes::Encoder encoder(rm);
    while (B.size() != size && !encoder.isEnd()) { //!!!!!!
        std::vector<char> codeWord;
        while (!encoder.isEnd()) {
            codeWord = encoder.next();
            if (algorithms::hammingWeight(codeWord) == weight && !vInsideVv(usedCodeWords, codeWord)) {
                usedCodeWords.push_back(codeWord);
                break;
            }
        }
        for (size_t j = 0; j < encoder.stage().size(); ++j) {
            std::cout << static_cast<int>(encoder.stage()[j]) << " ";
        }
        std::cout << ": ";
        for (size_t j = 0; j < codeWord.size(); ++j) {
            std::cout << static_cast<int>(codeWord[j]) << " ";
        }
        std::cout << std::endl;
    }
    return rm;
}

} //namespace attackSupporters
} // namespace codes
