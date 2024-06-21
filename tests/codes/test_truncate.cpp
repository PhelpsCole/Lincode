#include "linear_code.h"

int main(void) {
    std::vector<std::vector<char>> basis = {{1, 0, 0, 0, 1, 0, 1},
                                            {0, 1, 0, 0, 1, 1, 1},
                                            {0, 0, 1, 0, 1, 1, 0},
                                            {0, 0, 0, 1, 0, 1, 1}};
    std::vector<unsigned long long> columns{4, 5, 6};
    codes::Lincode code(basis);
    code.printVisualCode();
    std::cout << "Truncated by 4, 5, 6 columns:" << std::endl;
    codes::Lincode res(code.truncate(columns));
    res.printVisualCode();
    columns = {4, 5};
    std::cout << "Truncated by 4, 5 columns with deleting nulls:" << std::endl;
    res = code.truncate(columns, true);
    res.printVisualCode();
    return 0;
}