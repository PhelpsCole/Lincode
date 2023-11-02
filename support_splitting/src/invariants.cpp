#include "invariants.h"

namespace codes {
namespace invariants {
namespace support {
std::string invariantString(const std::vector<size_t> &spectr) {
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

std::string invariantWeightHullSupporter(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    return invariantString(hull.spectrum());
}

std::string invariantWeightHullHadSquareSupporter(const codes::Lincode &code) {
    codes::Lincode hullHadSquare = codes::hadamardProduct(code, code).hull();
    return invariantString(hullHadSquare.spectrum());
}

std::string invariantWeightBasisSupporter(const codes::Lincode &code) {
    //codes::Lincode punct = code.hull();
    return invariantString(code.spectrum_basis());
}

std::string invariantHullSizeSupporter(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    size_t size = hull.size();
    return std::to_string(size) + ",";
}

std::string invariantConvecter(const codes::Lincode &code,
                               const std::vector<size_t> &columns,
                               std::function<std::string(const codes::Lincode &)> invariantSup) {
    if (columns.size() == 0) {
        return invariantSup(code);
    }
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

std::string invariantConvecterMinRM(const codes::Lincode &code,
                                    const std::vector<size_t> &columns,
                                    std::function<std::string(const codes::Lincode &)> invariantSup) {
    std::vector<size_t> rmSizes = codes::rmSizes(code);
    size_t r = rmSizes[0], m = rmSizes[1];
    codes::Lincode newCode(code);
    if (m == 0) {
        return "Not RM-code for invariant";
    }
    if (r != 1) {
        if (2*r + 1 > m) {
            r = m - r - 1;
            newCode.dual();
        }
        size_t q = m / r;
        if (q * r == m) {
            --q;
        }
        if (m - r * q != 1 && m < r * (q - 1) + 1) {
            newCode = codes::hadPower(code, q);
            newCode.dual();
        }
    }
    newCode.puncture(columns);
    return invariantSup(newCode);
    
}

} // namespace support

std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecter(code, columns,
                              codes::invariants::support::invariantWeightHullSupporter);
}

std::string invariantWeightHullMinRM(const codes::Lincode &code,
                                     const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterMinRM(code, columns,
                                   codes::invariants::support::invariantWeightHullSupporter);
}

std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecter(code, columns,
                              codes::invariants::support::invariantHullSizeSupporter);
}

std::string invariantHullSizeMinRM(const codes::Lincode &code,
                                   const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterMinRM(code, columns,
                                   codes::invariants::support::invariantHullSizeSupporter);
}

std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecter(code, columns,
                              codes::invariants::support::invariantWeightBasisSupporter);
}

std::string invariantWeightHullHadSquare(const codes::Lincode &code,
                                 const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecter(code, columns,
                              codes::invariants::support::invariantWeightHullHadSquareSupporter);
}
} //namespace invariants
} //namespace codes