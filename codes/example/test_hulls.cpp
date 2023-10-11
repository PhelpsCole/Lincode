#include "codes.h"

int main(void) {
    std::vector<std::vector<char>> basis = {{1, 0, 0, 0, 1, 0, 1},
                                            {0, 1, 0, 0, 1, 1, 1},
                                            {0, 0, 1, 0, 1, 1, 0},
                                            {0, 0, 0, 1, 0, 1, 1},};
    std::cout << "By basis code, dual and hull:" << std::endl;
    codes::Lincode code(basis);
    codes::Lincode dual(code);
    dual.dual();
    code.printCode();
    dual.printCode();
    code.hull().printCode();
    std::cout << "By basis code, hull and hull of permuted:" << std::endl;
    codes::Lincode rm(codes::RMCode(2, 6).toMatrix());
    rm.printCode();
    rm.hull().printCode();
    matrix::Matrix random(matrix::generateRandomNonSingular(rm.size(), rm.size()));
    matrix::Matrix rm_matr(rm.toMatrix());
    random *= rm_matr;
    codes::Lincode(random).hull().printCode();
    matrix::Matrix hull(codes::Lincode(random).hull().toMatrix());
    matrix::Matrix hull_perm(rm.hull().toMatrix());
    std::cout << matrix::isEqual(hull, hull_perm) << std::endl;
    return 0;
}