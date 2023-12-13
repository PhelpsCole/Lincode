#include "linear_code.h"

namespace codes {

bool incorrect_basis(std::vector<std::vector<char>> &basis) {
    unsigned long long k = basis.size();
    unsigned long long n = basis[0].size();
    matrix::Matrix m(basis);
    return k > n || m.rank() != k;
}

Lincode::Lincode(unsigned long long _k, unsigned long long _n)
    : k(_k), n(_n) {
    basis = std::vector<std::vector<char>>();
    for (unsigned long long i = 0; i < k; ++i) {
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
    for (unsigned long long i = 0; i < k; ++i) {
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

unsigned long long Lincode::len() const {
    return n;
}

unsigned long long Lincode::size() const {
    return k;
}

unsigned long long Lincode::min_dist() const {
    matrix::Matrix code_matr(basis);
    code_matr.gaussElimination();
    unsigned long long min_weight = n;
    for (unsigned long long i = 0; i < code_matr.rows(); ++i) {
        std::vector<char> v = code_matr.row(i);
        unsigned long long weight = algorithms::hammingWeight(v);
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
std::vector<unsigned long long> Lincode::spectrum() const {
    std::vector<unsigned long long> spect(n);
    std::vector<char> vec(k, 0);
    std::pair<std::vector<char>, unsigned long long> weight_vec = std::make_pair(vec, 0);
    matrix::Matrix code_matr = toMatrix();
    while (weight_vec.second != k) {
        algorithms::addToBinWeightVector(weight_vec, 1);
        std::vector<char> res = code_matr.multiplyVectorByMatrix(weight_vec.first);
        unsigned long long weight = algorithms::hammingWeight(res);
        if (weight != 0) {
            ++spect[weight - 1];
        }
    }
    return spect;
}

std::string Lincode::stringSpectrum() const {
    std::ostringstream ss;
    ss << "{";
    std::vector<unsigned long long> spectr = spectrum();
    for (unsigned long long i = 0; i < spectr.size(); ++i) {
        if (spectr[i]) {
            ss << "[" << i + 1 << ":" << std::to_string(spectr[i]) << "];";    
        }
    }
    ss << "}";
    return ss.str();
}

std::vector<unsigned long long> Lincode::spectrum_basis() const {
    std::vector<unsigned long long> spect(n);
    for (unsigned long long i = 0; i < k; ++i) {
        unsigned long long weight = algorithms::hammingWeight(basis[i]);
        if (weight != 0) {
            ++spect[weight - 1];
        }
    }
    return spect;
}

Lincode Lincode::punctured(const std::vector<unsigned long long> &columns) const {
    Lincode copy(*this);
    copy.puncture(columns);
    return copy;
}

Lincode Lincode::punctured(unsigned long long column) const {
    Lincode copy(*this);
    copy.puncture(column);
    return copy;
}

Lincode Lincode::truncated(std::vector<unsigned long long> &columns) const {
    Lincode copy(*this);
    copy.truncate(columns);
    return copy;
}

void Lincode::printCodeSizes() const {
    std::cout << "k = " << k << ", n = " << n << std::endl;
}

void Lincode::printCode() const {
    printCodeSizes();
    for (unsigned long long i = 0; i < k; ++i) {
        for (unsigned long long j = 0; j < n; ++j) {
            std::cout << static_cast<int>(basis[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

void Lincode::printVisualCode(unsigned long long blocks_num) const {
    printCodeSizes();
    for (unsigned long long i = 0; i < k; ++i) {
        for (unsigned long long j = 0; j < n; ++j) {
            if (j && j % (n / blocks_num) == 0) {
                std::cout << "|";
            }
            if (basis[i][j] == 0) {
                std::cout << "-";
            } else if (basis[i][j] == 1) {
                std::cout << "+";
            }
        }
        std::cout << std::endl;
    }
}

} // namespace codes