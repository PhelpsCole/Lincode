#include "code_attacks.h"

int main() {
    codes::Lincode shortened({{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
						      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
						      {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
						      {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
						      {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
						      {1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
						      {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1}});
    std::cout << "Shortened code:" << std::endl;
    shortened.printCode();
    std::cout << "START decomposeShortenedCode" << std::endl;
    std::vector<codes::Lincode> vecDec =
    	codes::attackSupporters::decomposeShortenedCode(shortened, 2, 4);
    std::cout << "END decomposeShortenedCode" << std::endl;
    std::cout << "Vector of shortened codes:" << std::endl;
    for (size_t i = 0; i < vecDec.size(); ++i) {
    	vecDec[i].printCode();
    }
    return 0;
}