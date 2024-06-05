#include "code_attacks.h"
#include "algorithms.h"
#include <chrono>
#include <ctime>

void printVector(const std::vector<unsigned long long> &v, bool withNums = false) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (withNums) {
            std::cout << "[" << i << "]:";
        }
        if (i == v.size() / 3 * 2 ||
            i == v.size() / 3) {
            std::cout << "|";
            std::cout << std::endl;
        }
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

bool testRunner(const matrix::Matrix &rmCode,
                unsigned long long k1,
                unsigned long long k2,
                unsigned long long coutNum,
                unsigned long long minWeight,
                bool hullMode, bool byMinWeight, bool specialCase) {
    std::cout << "START testRunner" << std::endl;
    std::cout << k1 << " " << k2 << std::endl;
    std::cout << coutNum << " " << minWeight << std::endl;
    std::cout << "START hadPowerCounter" << std::endl;
    std::vector<unsigned long long>
    counter(codes::attackSupporters::hadPowerCounter(rmCode, coutNum, k2, hullMode,
                                                     byMinWeight, minWeight, 10, specialCase));
    std::cout << std::endl;
    std::cout << "END hadPowerCounter" << std::endl;
    {
        std::vector<unsigned long long> permVec(counter);
        permVec = algorithms::sorts::selectionSort(permVec,
                                                   [](const unsigned long long &a,
                                                      const unsigned long long &b)
                                                   { return a >= b; });
        for (unsigned long long i = 0; i < permVec.size(); ++i) {
            if (i != 0 && i % (permVec.size() / 3) == 0) {
                std::cout << "|";
                std::cout << std::endl;
            }
            std::cout << permVec[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Counter by all columns:" << std::endl;
    unsigned long long max = 0;
    unsigned long long min = 0;
    unsigned long long cnt = 0;
    bool correct = true;
    // 2r == m
    if (specialCase) {
        for (unsigned long long i = 0; i < counter.size(); ++i) {
            if (i != 0 && i % (counter.size() / 3) == 0) {
                std::cout << "|";
                std::cout << std::endl;
            }
            if (i < 2 * counter.size() / 3) {
                if (min == 0 || counter[i] < min) {
                    min = counter[i];
                }
            } else {
                if (min < counter[i]) {
                    correct = false;
                    ++cnt;
                }
                if (counter[i] > max) {
                    max = counter[i];
                }
            }
            std::cout << counter[i] << " ";
        }
    } else {
        for (unsigned long long i = 0; i < counter.size(); ++i) {
            if (i != 0 && i % (counter.size() / 3) == 0) {
                std::cout << "|";
                std::cout << std::endl;
            }
            if (i < 2 * counter.size() / 3) {
                if (counter[i] > max) {
                    max = counter[i];
                }
            } else {
                if (max > counter[i]) {
                    correct = false;
                    ++cnt;
                    if (min == 0 || counter[i] < min) {
                        min = counter[i];
                    }
                }
            }
            std::cout << counter[i] << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Maximum in blocks 1-2 is " << max << std::endl;
    std::cout << "Minimum in block 3 is " << min << std::endl;
    if (correct) {
        std::cout << "Counter correctly separates blocks" << std::endl;
        return true;
    } else {
        std::cout << "Counter doesn't separates blocks" << std::endl;
        std::cout << "Number of incorrect cols in 3 is " << cnt << std::endl;
        if (k2 < rmCode.cols() / 3 - cnt + 1) {
            std::cout << "Completed with partly separation" << std::endl;
            return true;
        }
    }
    std::cout << "END testRunner" << std::endl;
    return false;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    bool byMinWeight = false;
    bool permMode = false;
    size_t coutNum = 0;
    size_t step = 0;
    bool hullMode = false;
    if (argc < 3) {
        std::cout << "Input args in format: r m hullMode byMinWeight permMode coutNum step" << std::endl;
        //std::cout << "Where mode=0 -- subblocks 2-4, mode=1 -- modRM matr" << std::endl;
        std::cout << "Where permMode=1 -- *= P and permMode=0 --default" << std::endl;
        std::cout << "If step!=0 it continues inf times with step" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << hullMode << " " << byMinWeight << " "
                  << permMode << " " << coutNum << " " << step << std::endl;
        std::cout << "By 3: r m hullMode byMinWeight " << permMode
                  << " " << coutNum << " " << step << std::endl;
        std::cout << "By 4: r m hullMode byMinWeight permMode " << coutNum << " " << step << std::endl;
        std::cout << "By 5: r m hullMode byMinWeight permMode coutNum " << step << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        hullMode = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        hullMode = std::stoi(argv[3]);
        byMinWeight = std::stoi(argv[4]);
    } else if (argc == 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        hullMode = std::stoi(argv[3]);
        byMinWeight = std::stoi(argv[4]);
        permMode = std::stoi(argv[5]);
    } else if (argc == 7) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        hullMode = std::stoi(argv[3]);
        byMinWeight = std::stoi(argv[4]);
        permMode = std::stoi(argv[5]);
        coutNum = std::stoi(argv[6]);
    } else if (argc >= 8) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        hullMode = std::stoi(argv[3]);
        byMinWeight = std::stoi(argv[4]);
        permMode = std::stoi(argv[5]);
        coutNum = std::stoi(argv[6]);
        step = std::stoi(argv[7]);
    }
    unsigned long long k1, k2, minWeight;
    k1 = codes::codeSizeFromRM(r, m);
    k2 = codes::codeSizeFromRM(r - 1, m);
    unsigned long long startCoutNum = coutNum;
    minWeight = 1 << (m - r + 1);
    size_t amount = 10;
    for (size_t i = 0; i < amount; ++i) {
        std::cout << "START pqsigRMSubblockGenerator" << std::endl;
        matrix::Matrix subblocks = codes::pqsigRMSubblockGenerator(r, m, permMode);
        std::cout << "END pqsigRMSubblockGenerator" << std::endl;
        if (codes::attackSupporters::testSSASubblocks(subblocks, r, m, i)) {
            std::cout << "Found by SSA" << std::endl;
            ++amount;
            //continue;
        } else {
            std::cout << "SSA failed" << std::endl;
        }
        codes::Lincode hull(subblocks);
        if (hullMode) {
            hull.printCodeSizes();
            hull = hull.hull();
            if (m <= 6) {
                hull.printVisualCode(3);
            }
        }
        coutNum = startCoutNum;
        bool specialCase = 2*r == m;
        if (!step) {
            testRunner(hull.toMatrix(), k1, k2, coutNum,
                       minWeight, hullMode, byMinWeight, specialCase);
        } else {
            bool firstRun = true;
            do {
                {
                    auto now = std::chrono::system_clock::now();
                    std::time_t time = std::chrono::system_clock::to_time_t(now);
                    std::cout << "Started computation at " << std::ctime(&time) << std::endl;
                }
                if (!firstRun) {
                    coutNum += step;
                } else {
                    firstRun = false;
                }
                std::cout << "Counter = " << coutNum << std::endl;
            } while (coutNum != 100000 &&
                     !testRunner(hull.toMatrix(), k1, k2, coutNum,
                                 minWeight, hullMode, byMinWeight, specialCase));
            {
                auto now = std::chrono::system_clock::now();
                std::time_t time = std::chrono::system_clock::to_time_t(now);
                std::cout << "Completed computation at " << std::ctime(&time) << std::endl;
            }
        }
    }
    return 0;
}