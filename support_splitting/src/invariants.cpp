#include "invariants.h"

namespace codes {
namespace invariants {
namespace support {
std::string invariantWeightHullSupporter(const codes::Lincode &code) {
    codes::Lincode punct = code.hull();
    std::vector<size_t> spectr = punct.spectrum();
    std::ostringstream ss;
    ss << "{";
    for (size_t i = 0; i < spectr.size(); ++i) {
        if (spectr[i]) {
            ss << "[" << i + 1 << ":" << std::to_string(spectr[i]) << "];";    
        }
    }
    ss << "}";
    return ss.str();
}

std::string invariantWeightBasisSupporter(const codes::Lincode &code) {
    //codes::Lincode punct = code.hull();
    std::vector<size_t> spectr = code.spectrum_basis();
    std::ostringstream ss;
    ss << "{";
    for (size_t i = 0; i < spectr.size(); ++i) {
        if (spectr[i]) {
            ss << "[" << i + 1 << ":" << std::to_string(spectr[i]) << "];";    
        }
    }
    ss << "}";
    return ss.str();
}

std::string invariantHullSizeSupporter(const codes::Lincode &code) {
    size_t size = code.size();
    return std::to_string(size) + ",";
}

std::string invariantConvecter(const codes::Lincode &code,
                               const std::vector<size_t> &columns,
                               std::function<std::string(const codes::Lincode &)> invariantSup) {
    std::vector<size_t> spectr;
    std::vector<codes::Lincode> codesVec = {code};
    for (size_t i = 0; i < columns.size(); ++i) {
        std::vector<codes::Lincode> codesNewVec(2 * codesVec.size());
        for (size_t j = 0; j < codesVec.size(); ++j) {
            codesNewVec[2 * j] = codesVec[j].punctured(columns[i]);
            codesNewVec[2 * j + 1] = codesVec[j];
            codesNewVec[2 * j + 1].dual();
            codesNewVec[2 * j + 1].puncture(columns[i]);
        }
        codesVec = codesNewVec;
    }
    std::string ans;
    for (size_t i = 0; i < codesVec.size(); ++i) {
        ans += invariantSup(codesVec[i]);
    }
    return ans;
    
}

} // namespace support
namespace indeep {

std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns) {
    codes::Lincode punct = code.punctured(columns);
    std::vector<size_t> spectr = punct.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

// Bad for RM
std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns) {
    codes::Lincode punct = code.punctured(columns);
    codes::Lincode hull = punct.hull();
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

std::string invariantWeightHullBasis(const codes::Lincode &code,
                                     const std::vector<size_t> &columns) {
    codes::Lincode punct = code.punctured(columns);
    codes::Lincode hull = punct.hull();
    //hull.printCode();
    std::vector<size_t> spectr = hull.spectrum_basis();
    std::ostringstream ss;
    for (size_t i = 0; i < spectr.size(); ++i) {
        ss << std::to_string(spectr[i]) << ";";
    }
    return ss.str();
}

//Bad invariant
std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns) {
    codes::Lincode punct = code.punctured(columns);
    codes::Lincode hull = punct.hull();
    //hull.printCode();
    return std::to_string(hull.size());
}

//Bad invariant
std::string invariantHullHadSquareSize(const codes::Lincode &code,
                                       const std::vector<size_t> &columns) {
    codes::Lincode punct = code.punctured(columns);
    matrix::Matrix hmatr(punct.toMatrix());
    hmatr.hadamardProduct(hmatr);
    codes::Lincode hull = codes::Lincode(hmatr);
    hull = hull.hull();
    //hull.printCode();
    return std::to_string(hull.size());
}

std::string invariantWeightHullHadSquare(const codes::Lincode &code,
                                         const std::vector<size_t> &columns) {
    codes::Lincode punct = code.punctured(columns);
    codes::Lincode hull = punct.hull();
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
} //namespace indeep


std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecter(code, columns,
                              codes::invariants::support::invariantWeightHullSupporter);
}


std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecter(code, columns,
                              codes::invariants::support::invariantHullSizeSupporter);
}

std::string invariantWeightBasis(const codes::Lincode &code,
                                const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecter(code, columns,
                              codes::invariants::support::invariantWeightBasisSupporter);
}

} //namespace invariants
} //namespace codes