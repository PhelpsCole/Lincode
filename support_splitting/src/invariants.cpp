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