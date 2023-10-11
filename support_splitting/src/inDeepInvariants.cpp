#include "inDeepSSA.h"

namespace codes {
namespace indeep {
namespace invariants {

std::string invariantWeightBasis(codes::Lincode code,
                                 const std::vector<size_t> &columns) {
    code.puncture(columns);
    std::vector<size_t> spectr = code.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

// Bad for RM
std::string invariantWeightHull(codes::Lincode code,
                                const std::vector<size_t> &columns) {
    code.puncture(columns);
    codes::Lincode hull = code.hull();
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

std::string invariantWeightHullBasis(codes::Lincode code,
                              const std::vector<size_t> &columns) {
    code.puncture(columns);
    codes::Lincode hull = code.hull();
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

//Bad invariant
std::string invariantHullSize(codes::Lincode code,
                              const std::vector<size_t> &columns) {
    code.puncture(columns);
    codes::Lincode hull = code.hull();
    //hull.printCode();
    return std::to_string(hull.size());
}

//Bad invariant
std::string invariantHullHadSquareSize(codes::Lincode code,
                                       const std::vector<size_t> &columns) {
    code.puncture(columns);
    matrix::Matrix hmatr(code.toMatrix());
    hmatr.hadamardProduct(hmatr);
    codes::Lincode hull = codes::Lincode(hmatr);
    hull = hull.hull();
    //hull.printCode();
    return std::to_string(hull.size());
}

std::string invariantWeightHullHadSquare(codes::Lincode code,
                                         const std::vector<size_t> &columns) {
    code.puncture(columns);
    codes::Lincode hull = code.hull();
    matrix::Matrix hmatr(hull.toMatrix());
    hmatr.hadamardProduct(hmatr);
    hull = codes::Lincode(hmatr);
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

} //namespace invariants
} //namespace indeep
} //namespace codes
