#include "support_splitting.h"
#include "invariants.h"
#include "rm_code.h"

int main(void) {
    std::cout << "Input r and m parameters of your Reed-Muller code:" << std::endl;
    size_t r, m;
    std::cin >> r >> m;
    codes::RMCode rm(r, m);
    codes::Lincode rm_code(rm.toMatrix());
    //std::cout << "Inputed code:" << std::endl;
    //rm_code.printCode();
    //std::cout << "Dual of inputed code:" << std::endl;
    codes::Lincode dual = rm_code;
    //dual.dual();
    //dual.printCode();
    //std::cout << "Hadamard square of inputed code:" << std::endl;
    codes::Lincode hadSquare = hadamardProduct(rm_code, rm_code);
    //hadSquare.printCode();
    //std::cout << "Dual of hadamard square" << std::endl;
    //dual = hadSquare;
    //dual.dual();
    //dual.printCode();
    //std::cout << "Hadamard squares of punctured codes of inputted:" << std::endl;
    //for (size_t i = 0; i < rm_code.len(); ++i) {
      //codes::Lincode punct = rm_code.punctured(i);
      //codes::Lincode hadSquare = hadamardProduct(punct, punct);
      //std::vector<size_t> cols = {i};
      //hadSquare.printCode();
      //std::cout << "[" << i << "]: " << codes::invariants::invariantWeightHullHadSquare(rm_code, cols) << std::endl;
    //}
    //std::cout << "Hull of inputed code:" << std::endl;
    codes::Lincode hull = rm_code.hull();
    //hull.printCode();
    std::cout << "Hull of hadamard square:" << std::endl;
    hadSquare.hull().printCodeSizes();
    //hadSquare.hull().printCode();
    std::cout << "Hadamard square of hull:" << std::endl;
    hadamardProduct(hull, hull).printCodeSizes();
    //hadamardProduct(hull, hull).printCode();
}