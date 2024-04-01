#include "code_attacks.h"
#include <chrono>
#include <ctime>

matrix::Matrix testRunner(codes::Lincode modRM, size_t mode,
                          bool countSigmas = true, bool modRM_remover = false) {
    std::cout << "START testRunner:" << std::endl;
    std::cout << "modRM(r, m) code:" << std::endl;
    if (mode == 0) {
        modRM.printVisualCode();
    } else {
        modRM.printCodeSizes();
    }
    matrix::Matrix permMatr(0, 0);
    if (modRM_remover) {
        std::cout << "START modRM_remover" << std::endl;
        std::vector<matrix::Matrix> ans(codes::modRM_remover(modRM, true, countSigmas));
        if (ans.size() == 0) {
            std::cerr << "Wrong matrix and zero result" << std::endl;
            return matrix::Matrix(0, 0);
        }
        permMatr = ans[0];
        std::cout << "END modRM_remover" << std::endl;
        if (mode == 0) {
            std::cout << "Perm matrix:" << std::endl;
            permMatr.printMatrix();
        }
        std::cout << "Perm matrix rank: " << permMatr.rank() << std::endl;
        std::cout << "modRM(r, m)*P:" << std::endl;
        modRM = modRM.toMatrix() * permMatr;
    } else {
        std::cout << "START modRM_attack" << std::endl;
        modRM = codes::modRM_attack(modRM, true, countSigmas);
        std::cout << "END modRM_attack" << std::endl;
        permMatr = modRM.toMatrix();
    }
    std::cout << modRM.toMatrix().rank() << std::endl;
    if (mode == 0) {
        modRM.printVisualCode();
    } else {
        modRM.printCodeSizes();
    }
    std::cout << "END testRunner" << std::endl;
    return permMatr;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    size_t mode = 1;
    bool countSigmas = true;
    bool modRM_remover = false;
    if (argc < 3) {
        std::cout << "Input args in format: r m mode countSigmas modRM_remover" << std::endl;
        std::cout << "Where mode=0 -- print visual, mode=1 -- print sizes" << std::endl;
        std::cout << "Where modRM_remover=0 -- modRM_attack, modRM_remover=1 -- modRM_remover" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << mode << " " << countSigmas << " " << modRM_remover << std::endl;
        std::cout << "By 3: r m mode " << countSigmas << " " << modRM_remover << std::endl;
        std::cout << "By 4: r m mode countSigmas " << modRM_remover << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
        countSigmas = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
        countSigmas = std::stoi(argv[4]);
        modRM_remover = std::stoi(argv[5]);
    }
    if (!countSigmas && !modRM_remover) {
        std::cerr << "Unable to do modRM_attack without finding sigmas" << std::endl;
        return 0;
    }
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Started generating keys at " << std::ctime(&time) << std::endl;
    }
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode pqsigRMcode(pqsigRM);
    
    std::uniform_int_distribution<int> distrib(1, pqsigRMcode.len());
    std::random_device rd;
    std::mt19937 gen(rd());
    matrix::Matrix P = matrix::generateRandomPermutation(pqsigRMcode.len(), distrib(gen));

    matrix::Matrix G(pqsigRMcode.toMatrix() * P);
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Ended generating keys at " << std::ctime(&time) << std::endl;
    }
    P = testRunner(codes::Lincode(G), mode, countSigmas, modRM_remover);
    if (modRM_remover && !codes::isEquivalent(pqsigRMcode, G*P)
        || !modRM_remover && !codes::isEquivalent(pqsigRMcode, P)) {
        std::cout << "Same basises" << std::endl;
    } else {
        std::cout << "Basises are not the same" << std::endl;
    }
    return 0;
}