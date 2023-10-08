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

void Lincode::puncture(std::vector<size_t> &columns) {
    std::vector<size_t> mask(n);
    for (size_t row = 0; row < k; ++row) {
        std::vector<char> &v = basis[row];
        for (size_t i = 0; i < columns.size(); ++i) {
            v[columns[i]] = 0;
        }
    }
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

Lincode mcEliece(const Lincode &gCode) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    size_t k = gCode.size();
    size_t n = gCode.len();
    matrix::Matrix m(gCode.toMatrix());
    matrix::Matrix M = matrix::generateRandomNonSingular(k, k);

    std::uniform_int_distribution<int> distrib(1, n);
    matrix::Matrix P = matrix::generateRandomPermutation(n, distrib(gen));
    matrix::Matrix res = M * m * P;
    return Lincode(res);
}

} // namespace codes