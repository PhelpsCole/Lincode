#include <linear_code.h>

namespace codes {

Lincode::Lincode(size_t _k, size_t _n)
    : k(_k), n(_n) {
    basis = std::vector<std::vector<char>>();
    for (size_t i = 0; i < k; ++i) {
        basis.push_back(std::vector<char>(n));
    }
}

Lincode::Lincode(std::vector<std::vector<char>> &_basis)
    : k(_basis.size()), n(_basis[0].size()), basis(_basis) {}

Lincode::Lincode(Lincode &other)
    : k(other.size()), n(other.len()) {
    basis = std::vector<std::vector<char>>(k);
    std::vector<std::vector<char>> other_basis = other.getBasis();
    for (size_t i = 0; i < k; ++i) {
        basis[i].insert(basis[i].begin(), other_basis[i].begin(), other_basis[i].end());
    }
}

Lincode::Lincode(matrix::Matrix &matr)
    : k(matr.rows()), n(matr.cols()), basis(matr.toVectors()) {}

size_t Lincode::len() const {
    return n;
}

size_t Lincode::size() const {
    return k;
}
size_t Lincode::min_dist() const {
    matrix::Matrix code_matr(basis);
    code_matr.echelon();
    size_t min_weight = n;
    for (size_t i = 0; i < code_matr.rows(); ++i) {
        std::vector<char> v = code_matr.row(i);
        size_t weight = algorithms::hammingWeight(v);
        if (weight < min_weight) {
            min_weight = weight;
        }
    }
    return min_weight;
}

std::vector<std::vector<char>> &Lincode::getBasis() {
    return basis;
}

Lincode Lincode::dual() {
    matrix::Matrix m(basis);
    m.orthogonal();
    k = m.rows();
    n = m.cols();
    basis = m.toVectors();
    return *this;
}

void Lincode::printCode() const {
    std::cout << "k = " << k << ", n = " << n << std::endl;
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << static_cast<int>(basis[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

} // namespace codes