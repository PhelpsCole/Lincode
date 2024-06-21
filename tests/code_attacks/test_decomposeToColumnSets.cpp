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
    std::cout << "START columnSetsVector" << std::endl;
    std::vector<std::vector<unsigned long long>> vecDec =
    	codes::attackSupporters::decomposeToColumnSets(shortened, 2, 4);
    std::cout << "END columnSetsVector" << std::endl;
    std::cout << "Sets of columns:" << std::endl;
    for (size_t i = 0; i < vecDec.size(); ++i) {
    	for (size_t j = 0; j < vecDec[i].size(); ++j) {
    		std::cout << vecDec[i][j] << " ";
    	}
    	std::cout << std::endl;
    }
    return 0;
}