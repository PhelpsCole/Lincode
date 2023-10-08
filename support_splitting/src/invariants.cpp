#include "support_splitting.h"

namespace codes {
namespace invariants {

std::string invariantWeightBasis(const codes::Lincode &code) {
    std::vector<size_t> spectr = code.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

std::string invariantWeightHull(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

std::string invariantWeightHullBasis(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

std::string invariantHullSize(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    //hull.printCode();
    return std::to_string(hull.size());
}

std::string invariantHullHadSquareSize(const codes::Lincode &code) {
    matrix::Matrix hmatr(code.toMatrix());
    hmatr.hadamardProduct(hmatr);
    codes::Lincode hull = codes::Lincode(hmatr);
    hull = hull.hull();
    //hull.printCode();
    return std::to_string(hull.size());
}

std::string invariantWeightHullHadSquare(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    matrix::Matrix hmatr(hull.toMatrix());
    hmatr.hadamardProduct(hmatr);
    hull = codes::Lincode(hmatr);
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

} //namespace invariants
}//namespace codes