#include "code_attacks.h"

matrix::Matrix matrixGenerator(size_t r, size_t m, size_t permMode) {
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

    return subblock;
}

void printVector(const std::vector<unsigned long long> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void testRunner(matrix::Matrix rmCode,
                unsigned long long k1, unsigned long long k2) {
    std::cout << "START testRunner:" << std::endl;
    std::cout << k1 << " " << k2 << " " << rmCode.rows() << std::endl;
    std::cout << "sublock with RM(r, m) and RM(r-1, m)^s2:" << std::endl;
    rmCode.printVisualMatrix(3);

    std::cout << "START separatingGaussElimination" << std::endl;
    std::vector<std::vector<unsigned long long>>
    result(codes::attackSupporters::separatingGaussElimination(rmCode, k1, k2));
    std::cout << "END separatingGaussElimination" << std::endl;

    std::cout << "Sublock:" << std::endl;
    rmCode.printVisualMatrix(3, true);
    std::cout << "First block columns [" << result[0].size() << "]:" << std::endl;
    printVector(result[0]);
    std::cout << "First block rows [" << result[1].size() << "]:" << std::endl;
    printVector(result[1]);
    std::cout << "First block columns [" << result[2].size() << "]:" << std::endl;
    printVector(result[2]);
    std::cout << "First block rows [" << result[3].size() << "]:" << std::endl;
    printVector(result[3]);
    std::cout << "First block columns [" << result[4].size() << "]:" << std::endl;
    printVector(result[4]);
    std::cout << "First block rows [" << result[5].size() << "]:" << std::endl;
    printVector(result[5]);

    std::cout << "START rowsIntersections" << std::endl;
    result = codes::attackSupporters::rowsIntersections(rmCode, result[1], result[3], result[5]);
    std::cout << "END rowsIntersections" << std::endl;
    std::cout << "Vectors of intersections:" << std::endl;
    std::cout << "(1,1), [" << result[0].size() << "]: ";
    printVector(result[0]);
    std::cout << "(1,2), [" << result[1].size() << "]: ";
    printVector(result[1]);
    std::cout << "(1,3), [" << result[2].size() << "]: ";
    printVector(result[2]);
    std::cout << "(2,2), [" << result[3].size() << "]: ";
    printVector(result[3]);
    std::cout << "(2,3), [" << result[4].size() << "]: ";
    printVector(result[4]);
    std::cout << "(3,3), [" << result[5].size() << "]: ";
    printVector(result[5]);
    std::cout << "Intersection of all:" << std::endl;
    std::vector<unsigned long long> indexes(5);
    size_t ind = 0;
    for (size_t i = 0; i < result[5].size(); ++i) {
        bool endSearch = false;
        for (size_t j = 0; j < indexes.size(); ++j) {
            while (result[5][i] > result[j][indexes[j]]) {
                ++indexes[j];
                if (indexes[j] == result[j].size()) {
                    endSearch = true;
                    break;
                }
            }
            if (endSearch) {
                break;
            }
        }
        if (endSearch) {
            break;
        }
        if (result[5][i] == result[0][indexes[0]] && result[5][i] == result[1][indexes[1]]
            && result[5][i] == result[2][indexes[2]] && result[5][i] == result[3][indexes[3]]
            && result[5][i] == result[4][indexes[4]]) {
            ++ind;
            std::cout << result[5][i] << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "[" << ind << "]" << std::endl;
    std::cout << "END testRunner" << std::endl;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    size_t permMode = 0;
    size_t mode = 0;
    size_t iterNum = 1;
    if (argc < 3) {
        std::cout << "Input args in format: r m permMode mode iterNum" << std::endl;
        std::cout << "Where permMode=0 -- *= P and permMode=1 --default" << std::endl;
        std::cout << "Where mode=0 -- print visual, mode=1 -- print sizes" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << permMode << " " << mode << " " << iterNum << std::endl;
        std::cout << "By 3: r m permMode" << mode << " " << iterNum << std::endl;
        std::cout << "By 4: r m permMode mode " << iterNum << std::endl;
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
        iterNum = std::stoi(argv[4]);
    }
    for (size_t i = 0; i < iterNum; ++i) {
        testRunner(matrixGenerator(r, m, permMode),
                   codes::codeSizeFromRM(r, m), codes::codeSizeFromRM(r - 1, m));
    }
    return 0;
}