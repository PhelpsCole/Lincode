#include "code_attacks.h"
#include "ssa_test.h"
#include <chrono>
#include <ctime>


namespace codes {
namespace attackSupporters {

bool testSSASubblocks(const matrix::Matrix &pqsigRMSubblock,
                      size_t r, size_t m, size_t cicleStep) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::string filename = "subblock(" + std::to_string(r) + "," + std::to_string(m)
                            + ")_";
    if (cicleStep != 0) {
        filename += std::to_string(cicleStep);
    } 
    std::cout << "Processing " << filename << "..." << std::endl;
    std::cout << "Started computation at " << std::ctime(&time) << std::endl;
    std::string symb;
    codes::SSAStructure s;

    if (2*r == m) {
        std::cout << "This algorithm doesn't separates in m = 2r case" << std::endl;
        return false;
    }

    codes::Lincode subblock(pqsigRMSubblock);
    subblock.dual();
    r = m - r;
    codes::Lincode startCode(subblock);
    std::vector<int> maxRMVector = codes::maxRMVector(r, m);
    if (maxRMVector.size() == 0) {
        throw std::invalid_argument("Wrong r and m parameters.");
    }
    for (unsigned long long i = 0; i < maxRMVector.size(); ++i) {
        if (maxRMVector[i] == -1) {
            if (i != 0) {
                symb += "-1|";
                subblock.dual();
            }
        } else {
            symb += std::to_string(maxRMVector[i]) + "|";
            subblock = codes::hadPower(subblock, maxRMVector[i]);

            s = codes::ssaStructure(subblock, codes::attackSupporters::INVARIANT_SIZE_ID,
                                    codes::attackSupporters::PREPROC_SIMPLE_ID);

            now = std::chrono::system_clock::now();
            time = std::chrono::system_clock::to_time_t(now);
            std::cout << "Completed computation of " << symb << " at " << std::ctime(&time) << std::endl;

            if (ssa_test::checkSubblockSignature(s, m)) {
                std::string tempFilename = filename + '_' + symb  
                                           + '_' + std::to_string(cicleStep) + "_found.txt";
                ssa_test::printSSAStructure(s, tempFilename);
                return true;
            } else {
                std::string tempFilename = filename + '_' + symb  
                                           + '_' + std::to_string(cicleStep) + "_continue.txt";
                ssa_test::printSSAStructure(s, tempFilename);
            }
        }
    }
    std::string tempFilename = filename + '_' + symb  
                               + '_' + std::to_string(cicleStep) + ".txt";
    std::string unFoundMatrix = filename + '_' + symb  
                               + "_matrix" + std::to_string(cicleStep) + ".txt";
    ssa_test::printSSAStructure(s, tempFilename);
    startCode.printCode(unFoundMatrix);
    return false;
}

} //namespace attackSupporters
} // namespace codes