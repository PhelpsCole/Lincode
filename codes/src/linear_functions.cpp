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

void Lincode::truncate(std::vector<unsigned long long> columns) {
    std::sort(columns.begin(), columns.end());
    std::vector<std::vector<char>> new_basis(k);
    for (unsigned long long row = 0; row < k; ++row) {
        unsigned long long ind = 0;
        std::vector<char> v(n - columns.size());
        for (unsigned long long i = 0; i < n; ++i) {
            if (i != columns[ind]) {
                v[i - ind] = basis[row][i];
            } else {
                ++ind;
            }
        }
        new_basis[row] = v;
    }
    n -= columns.size();
    basis = new_basis;
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