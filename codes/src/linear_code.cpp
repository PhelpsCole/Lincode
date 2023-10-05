#include <linear_code.h>

namespace codes {

Lincode::Lincode(size_t _k, size_t _n)
    : k(_k), n(_n) {
    basis = std::vector<std::vector<char>>();
    for (size_t i = 0; i < k; ++i) {
        basis.push_back(std::vector<char>(n));
    }
}

Lincode::Lincode(const std::vector<std::vector<char>> &_basis, bool check)
    : k(_basis.size()), n(_basis[0].size()), basis(_basis) {
    if (check && incorrect_basis(basis)) {
        throw std::invalid_argument("Incorrect inputted basis.");
    }
}

Lincode::Lincode(const Lincode &other, bool check)
    : k(other.size()), n(other.len()) {
    basis = std::vector<std::vector<char>>(k);
    if (check && incorrect_basis(basis)) {
        throw std::invalid_argument("Incorrect inputted basis.");
    }
    std::vector<std::vector<char>> other_basis = other.getBasis();
    for (size_t i = 0; i < k; ++i) {
        basis[i].insert(basis[i].begin(), other_basis[i].begin(), other_basis[i].end());
    }
}

Lincode::Lincode(const matrix::Matrix &matr, bool check)
    : k(matr.rows()), n(matr.cols()), basis(matr.toVectors()) {
    if (check && incorrect_basis(basis)) {
        throw std::invalid_argument("Incorrect inputted basis.");
    }
}

size_t Lincode::len() const {
    return n;
}

size_t Lincode::size() const {
    return k;
}

size_t Lincode::min_dist() const {
    std::vector<char> v(k);
    size_t min_weight = n;
    matrix::Matrix code_matr = toMatrix();
    for (size_t i = 0; i < static_cast<size_t>((1 << v.size()) - 1); ++i) {
        addToBinVector(v, 1);
        std::vector<char> res = code_matr.multiplyVectorByMatrix(v);
        size_t weight = algorithms::hammingWeight(res);
        if (weight < min_weight) {
            min_weight = weight;
        }
    }
    return min_weight;
}

matrix::Matrix Lincode::toMatrix() const {
    return matrix::Matrix(basis);
}

const std::vector<std::vector<char>> &Lincode::getBasis() const {
    return basis;
}

Lincode Lincode::hull() const {
    Lincode copy(*this);
    Lincode dual(copy);
    dual.dual();
    matrix::Matrix a(copy.toMatrix());
    matrix::Matrix b(dual.toMatrix());
    a.concatenateByColumns(b);
    a.convertToBasis();
    copy = Lincode(a);
    copy.dual();
    return copy;
}

//spect[i] - words of weight i + 1
std::vector<size_t> Lincode::spectrum() const {
    std::vector<size_t> spect(n);
    std::vector<char> v(k);
    matrix::Matrix code_matr = toMatrix();
    for (size_t i = 0; i < static_cast<size_t>((1 << v.size()) - 1); ++i) {
        addToBinVector(v, 1);
        std::vector<char> res = code_matr.multiplyVectorByMatrix(v);
        ++spect[algorithms::hammingWeight(res) - 1];
    }
    return spect;
}

std::vector<char> Lincode::encode(std::vector<char> &c) {
    if (c.size() != k) {
        throw std::invalid_argument("Incorrect size of message vector.");
    }
    std::vector<char> v(n);
    matrix::Matrix code_matr = toMatrix();
    return code_matr.multiplyVectorByMatrix(v);
}

void Lincode::dual() {
    matrix::Matrix m(basis);
    m.orthogonal();
    k = m.rows();
    n = m.cols();
    basis = m.toVectors();
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

bool incorrect_basis(std::vector<std::vector<char>> &basis) {
    size_t k = basis.size();
    size_t n = basis[0].size();
    matrix::Matrix m(basis);
    return k > n || m.rank() != k;
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

void addToBinVector(std::vector<char> &v, size_t n) {
    for (size_t i = 0; i < v.size() && n != 0; ++i) {
        v[i] += n % 2;
        if (v[i] == 2) {
            v[i] = 0;
            ++n;
        }
        n >>= 1;
    }
    if (n != 0) {
        int cnt = 0, prev_size = v.size();
        int tmp = n;
        while (tmp) {
            ++cnt;
            tmp >>= 1;
        }
        std::cout << cnt << std::endl;
        v.resize(prev_size + cnt);
        for (size_t i = 0; i < v.size() - prev_size; ++i) {
            v[prev_size + i] = n % 2;
            n >>= 1;
        }
    }
}

} // namespace codes