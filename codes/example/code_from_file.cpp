#include "codes.h"

// Problem with Cmake linkage
void testCodeFromFile(const std::string &filename, char c = ' ', char r = '\n') {
    std::cout << "STARTS testCodeFromFile" << std::endl;
    //std::string filename;
    //std::cout << "Input full path to your file with code:" << std::endl;
    //std::cin >> filename;
    std::cout << filename << std::endl;
    codes::Lincode code(matrix::matrFromFile(filename, c, r));
    code.printCodeSizes();
    //code.printVisualCode(4);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Input full path to your file with code" << std::endl;
        return 0;
    }
    testCodeFromFile(argv[1]);
    return 0;
}