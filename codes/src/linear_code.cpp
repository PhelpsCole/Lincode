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

Lincode::Lincode(std::string &str,  char tabs, char sep, bool check) {
    matrix::Matrix matr(str, tabs, sep);
    k = matr.rows();
    n = matr.cols();
    basis = matr.toVectors();
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
    matrix::Matrix code_matr(basis);
    code_matr.gaussElimination();
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
    unsigned long long size = static_cast<unsigned long long>((1 << v.size()) - 1);
    for (unsigned long long i = 0; i < size; ++i) {
        addToBinVector(v, 1);
        std::vector<char> res = code_matr.multiplyVectorByMatrix(v);
        size_t weight = algorithms::hammingWeight(res);
        if (weight != 0) {
            ++spect[weight - 1];
        }
    }
    return spect;
}

std::string Lincode::stringSpectrum() const {
    std::ostringstream ss;
    ss << "{";
    std::vector<size_t> spectr = spectrum();
    for (size_t i = 0; i < spectr.size(); ++i) {
        if (spectr[i]) {
            ss << "[" << i + 1 << ":" << std::to_string(spectr[i]) << "];";    
        }
    }
    ss << "}";
    return ss.str();
}

std::vector<size_t> Lincode::spectrum_basis() const {
    std::vector<size_t> spect(n);
    for (size_t i = 0; i < k; ++i) {
        size_t weight = algorithms::hammingWeight(basis[i]);
        if (weight != 0) {
            ++spect[weight - 1];
        }
    }
    return spect;
}

Lincode Lincode::punctured(const std::vector<size_t> &columns) const {
    Lincode copy(*this);
    copy.puncture(columns);
    return copy;
}

Lincode Lincode::punctured(size_t column) const {
    Lincode copy(*this);
    copy.puncture(column);
    return copy;
}

Lincode Lincode::truncated(std::vector<size_t> &columns) const {
    Lincode copy(*this);
    copy.truncate(columns);
    return copy;
}

void Lincode::printCodeSizes() const {
    std::cout << "k = " << k << ", n = " << n << std::endl;
}

void Lincode::printCode() const {
    printCodeSizes();
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
        v.resize(prev_size + cnt);
        for (size_t i = 0; i < v.size() - prev_size; ++i) {
            v[prev_size + i] = n % 2;
            n >>= 1;
        }
    }
}

} // namespace codes