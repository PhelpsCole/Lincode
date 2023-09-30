#include "rm_code.h"


int main() {
    unsigned int r = 3;
    unsigned int m = 3;

    ReedMullerCode reed_muller(r, m);

    for (const auto& basis_vector : reed_muller.getBasis()) {
        for (bool bit : basis_vector) {
            std::cout << bit << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}