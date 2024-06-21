#include "rm_code.h"


int main() {
    for (size_t m = 3; m < 21; ++m) {
        for (size_t r = 1; r < m; ++r) {
            std::vector<int> mrm  = codes::maxRMVector(r, m);
            std::cout << "(" << r << " " << m << "): ";
            for (size_t i = 0; i < mrm.size(); ++i) {
                std::cout << mrm[i] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}