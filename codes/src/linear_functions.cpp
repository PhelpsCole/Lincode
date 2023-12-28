#include <linear_code.h>

namespace codes {

std::vector<char> Lincode::encode(std::vector<char> &v) {
    if (v.size() != k) {
        throw std::invalid_argument("Incorrect size of message vector.");
    }
    matrix::Matrix code_matr = toMatrix();
    std::vector<char> res = code_matr.multiplyVectorByMatrix(v);
    return res;
}

void Lincode::basisView() {
    matrix::Matrix matr = toMatrix();
    matr.convertToBasis();
    k = matr.rows();
    n = matr.cols();
    basis = matr.toVectors();
}

void Lincode::dual() {
    matrix::Matrix m(basis);
    m.orthogonal();
    k = m.rows();
    n = m.cols();
    basis = m.toVectors();
}

void Lincode::puncture(const std::vector<unsigned long long> &columns) {
    for (unsigned long long row = 0; columns.size() && row < k; ++row) {
        std::vector<char> &v = basis[row];
        for (unsigned long long i = 0; i < columns.size(); ++i) {
            v[columns[i]] = 0;
        }
    }
    basisView();
}

void Lincode::puncture(unsigned long long column) {
    for (unsigned long long row = 0; row < k; ++row) {
        basis[row][column] = 0;
    }
}

Lincode Lincode::truncate(std::vector<unsigned long long> &columns, bool removeZeroes) const {
    matrix::Matrix A(toMatrix());
    matrix::Matrix B(0, 0);
    A.gaussElimination(false, columns);
    algorithms::sorts::selectionSort(columns,
                                     [](const unsigned long long &a,
                                        const unsigned long long &b) { return a <= b; });
    for (size_t i = 0; i < A.rows(); ++i) {
        bool nullMask = true;
        for (size_t j = 0; j < columns.size(); ++j) {
            if (A.at(i, columns[j]) != 0) {
                nullMask = false;
                break;
            }
        }
        if (nullMask) {
            if (removeZeroes) {
                std::vector<char> tmp(A.cols() - columns.size());
                std::vector<char> row(A.row(i));
                size_t indTmp = 0;
                size_t indCol = 0;
                for (size_t i = 0; i < row.size(); ++i) {
                    if (i == columns[indCol]) {
                        ++indCol;
                    } else {
                        tmp[indTmp++] = row[i];
                    }
                }
                B.insertRow(B.rows(), tmp);
            } else {
                B.insertRow(B.rows(), A.row(i));
            }
        }
    }
    return B;
}

Lincode Lincode::truncate(const std::vector<char> &mask, bool removeZeroes) const {
    std::vector<unsigned long long> columns;
    for (unsigned long long i = 0; i < mask.size(); ++i) {
        if (mask[i] != 0) {
            columns.push_back(i);
        }
    }
    return truncate(columns, removeZeroes);
}

Lincode sum(const Lincode &first, const Lincode &second) {
    matrix::Matrix a(first.toMatrix());
    matrix::Matrix b(second.toMatrix());
    a.concatenateByColumns(b);
    a.convertToBasis();
    return Lincode(a);
}

Lincode intersect(Lincode &first, Lincode &second) {
    first.dual();
    second.dual();
    matrix::Matrix a(first.toMatrix());
    matrix::Matrix b(second.toMatrix());
    a.concatenateByColumns(b);
    a.convertToBasis();
    first = Lincode(a);
    first.dual();
    return first;
}

Lincode hadamardProduct(const Lincode &first, const Lincode &second) {
    matrix::Matrix a(first.toMatrix());
    matrix::Matrix b(second.toMatrix());
    return Lincode(matrix::hadamardProduct(a, b));
}

// ToDo: bin power
Lincode hadPower(const Lincode &c, size_t power) {
    return codes::Lincode(hadPower(c.toMatrix(), power));
}

// True if c1 fully includes c2
bool linearDependence(const Lincode &c1, const Lincode &c2) {
    matrix::Matrix tmp(c1.toMatrix());
    tmp.concatenateByColumns(c2.toMatrix());
    codes::Lincode ans(tmp);
    ans.basisView();
    return ans.size() != c1.size();
}

// If c1 and c2 has same basis
bool isEquivalent(Lincode c1, Lincode c2) {
    c1.basisView();
    c2.basisView();
    return c1.size() == c2.size() && linearDependence(c1, c2);
}

Lincode mcEliece(const Lincode &gCode) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    unsigned long long k = gCode.size();
    unsigned long long n = gCode.len();
    matrix::Matrix m(gCode.toMatrix());
    matrix::Matrix M = matrix::generateRandomNonSingular(k, k);

    std::uniform_int_distribution<int> distrib(1, n);
    matrix::Matrix P = matrix::generateRandomPermutation(n, distrib(gen));
    matrix::Matrix res = M * m * P;
    return Lincode(res);
}

} // namespace codes