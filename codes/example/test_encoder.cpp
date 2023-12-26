#include "codes.h"
#include "encoder.h"

void printVector(const std::vector<char> &codeWord) {
    for (size_t i = 0; i < codeWord.size(); ++i) {
        std::cout << static_cast<int>(codeWord[i]) << " ";
    }
}

int main(void) {
    std::vector<std::vector<char>> basis = {{1, 0, 0, 0, 1, 0, 1},
                                            {0, 1, 0, 0, 1, 1, 1},
                                            {0, 0, 1, 0, 1, 1, 0},
                                            {0, 0, 0, 1, 0, 1, 1},};
    codes::Lincode code(basis);
    codes::Encoder encoder(code);
    while (!encoder.isEnd()) {
        std::vector<char> codeWord = encoder.next();
        printVector(encoder.stage());
        std::cout << ": ";
        printVector(codeWord);
        std::cout << std::endl;
    }
    return 0;
}