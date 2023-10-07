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
    std::vector<size_t> spectr = hull.spectrum();
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

} //namespace invariants
}//namespace codes