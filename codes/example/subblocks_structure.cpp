#include "codes.h"
#include <set>

// Problem with Cmake linkage
void testSubblockStructure(size_t r, size_t m, bool permMode, bool withHull) {
    std::cout << "STARTS testSubblockStructure" << std::endl;
    codes::RMCode rm(r, m);
    matrix::Matrix rmMatr(rm.toMatrix());
    matrix::Matrix zero(rmMatr.rows(), rmMatr.cols());
    matrix::Matrix subblock(rmMatr);
    matrix::Matrix row2(zero);
    subblock.concatenateByRows(zero);
    // (r,m)|0|(r,m)
    subblock.concatenateByRows(rmMatr);
    row2.concatenateByRows(rmMatr);
    // 0|(r,m)|(r,m)
    row2.concatenateByRows(rmMatr);
    subblock.concatenateByColumns(row2);

    rm = codes::RMCode(r - 1, m);
    rmMatr = rm.toMatrix();
    zero = matrix::Matrix(rmMatr.rows(), 2 * rmMatr.cols());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, rmMatr.cols());
    matrix::Matrix P = matrix::generateRandomPermutation(rmMatr.cols(), distrib(gen));

    rmMatr = rmMatr * P;

    // 0|0|(r-1,m)
    zero.concatenateByRows(rmMatr);
    subblock.concatenateByColumns(zero);

    matrix::Matrix M = matrix::generateRandomNonSingular(subblock.rows(), subblock.rows());
    std::uniform_int_distribution<int> distrib2(1, subblock.cols());
    P = matrix::generateRandomPermutation(subblock.cols(), distrib2(gen));
    if (!permMode) {
        subblock = M * subblock * P;
    } else {
        subblock = M * subblock;
    }

    if (withHull) {
        codes::Lincode hull(subblock);
        hull.printVisualCode(3);
        std::cout << "Dual:" << std::endl;
        hull.dual();
        hull.printVisualCode(3);
        hull = subblock;
        hull = hull.hull();
        std::cout << "Hull:" << std::endl;
        hull.printVisualCode(3);
        hull.basisView();
        hull.printVisualCode(3);

        size_t power = m / (m - r);
        std::cout << power << std::endl;
        //hull = subblock;
        //hull.dual();
        hull = hadPower(hull, power);
        hull.dual();
        hull.toMatrix().printVisualMatrix(3, true);
    }
    return;
    subblock.gaussElimination();

    std::vector<std::vector<unsigned long long>> sets;

    std::cout << "k = " << subblock.rows() << ", n = " << subblock.cols() << std::endl;
    size_t minWeight = (1 << (m - r + 1));
    std::cout << "min weight of last block: " << minWeight << std::endl;
    std::cout << "basis of last block: " << rmMatr.rows() << std::endl;
    for (size_t i = 0; i < subblock.rows(); ++i) {
        std::vector<char> tmp = subblock.row(i);
        size_t hamWeight = algorithms::hammingWeight(tmp);
            std::cout << i << ": ";
            for (size_t j = 0; j < tmp.size(); ++j) {
                if (j % rmMatr.cols() == 0 && j != 0) {
                    std::cout << "|";
                }
                if (tmp[j] == 0) {
                    std::cout << "-";
                } else {
                    std::cout << "+";
                }
            }
            std::cout << " [" << hamWeight << "]";
        if (minWeight == hamWeight) {
            std::cout << "!!!";
            std::vector<unsigned long long> setElems(hamWeight);
            size_t ind = 0;
            for (size_t j = 0; j < tmp.size(); ++j) {
                if (tmp[j] == 1) {
                    setElems[ind++] = j;
                }
            }
            sets.push_back(setElems);
        }
            std::cout << std::endl;
    }
    if (!permMode) {
        std::set<unsigned long long> set;
        for (size_t i = 0; i < sets.size(); ++i) {
            for (size_t j = 0; j < sets[i].size(); ++j) {
                std::cout << sets[i][j] << " ";
                set.insert(sets[i][j]);
            }
            std::cout << std::endl;
            for (auto elem : set) {
                std::cout << elem << " ";
            }
            std::cout << "[" << set.size() << "]" << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    size_t iter_num = 1;
    size_t r = 2, m = 7;
    bool permMode = 0;
    bool withHull = 0;
    if (argc < 3) {
        std::cout << "Input args in format: r m permMode withHull iter_num" << std::endl;
        std::cout << "Where permMode=0 is *= P" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << permMode << " " << withHull << " " << iter_num << std::endl;
        std::cout << "By 3: r m permMode " << withHull << " " << iter_num << std::endl;
        std::cout << "By 4: r m permMode withHull " << iter_num << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        permMode = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        permMode = std::stoi(argv[3]);
        withHull = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        permMode = std::stoi(argv[3]);
        withHull = std::stoi(argv[4]);
        iter_num = std::stoi(argv[5]);
    }
    for (size_t i = 0; i < iter_num; ++i) {
        testSubblockStructure(r, m, permMode, withHull);
    }
    return 0;
}