#include "invariants.h"

namespace codes {
namespace invariants {

invarType returnInvarById(size_t id) {
    switch(id) {
    case 0:
        return invariantHullSize;
    case 1:
        return invariantWeightHull;
    case 2:
        return invariantSimpleHullSize;
    case 3:
        return invariantHadSquareSize;
    case 4:
        return invariantWeightHadSquare;
    case 5:
        return invariantHullHadSquareSize;
    case 6:
        return invariantWeightHullHadSquare;
    case 7:
        return invariantWeightBasis;
    }
    return invariantHullSize;
}

std::string returnInvarNameById(size_t id) {
    switch(id) {
    case 0:
        return "invariantHullSize";
    case 1:
        return "invariantWeightHull";
    case 2:
        return "invariantSimpleHullSize";
    case 3:
        return "invariantHadSquareSize";
    case 4:
        return "invariantWeightHadSquare";
    case 5:
        return "invariantHullHadSquareSize";
    case 6:
        return "invariantWeightHullHadSquare";
    case 7:
        return "invariantWeightBasis";

    }
    return "invariantHullSize";
}

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

std::string invariantWeightSupporter(const codes::Lincode &code) {
    return invariantString(code.spectrum());
}

std::string invariantWeightBasisSupporter(const codes::Lincode &code) {
    //codes::Lincode punct = code.hull();
    return invariantString(code.spectrum_basis());
}

std::string invariantHullSizeSupporter(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    return std::to_string(hull.size()) + ",";
}

std::string invariantSizeSupporter(const codes::Lincode &code) {
    return std::to_string(code.size()) + ",";
}

std::string invariantConvecterSendrier(const codes::Lincode &code,
                                       const std::vector<size_t> &columns,
                                       std::function<std::string(const codes::Lincode &)>
                                       invariantSup) {
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

std::string invariantConvecterSimple(const codes::Lincode &code,
                                     const std::vector<size_t> &columns,
                                     std::function<std::string(const codes::Lincode &)>
                                     invariantSup) {
    codes::Lincode newCode(code.hull());
    newCode.puncture(columns);
    return invariantSup(newCode);  
}

std::string invariantConvecterMinRM(const codes::Lincode &code,
                                    const std::vector<size_t> &columns,
                                    std::function<std::string(const codes::Lincode &)>
                                    invariantSup) {
    codes::Lincode newCode(codes::minRM(code));
    newCode.puncture(columns);
    return invariantSup(newCode);  
}

std::string invariantConvecterHadSquareSimple(const codes::Lincode &code,
                                              const std::vector<size_t> &columns,
                                              std::function<std::string(const codes::Lincode &)>
                                              invariantSup) {
    codes::Lincode newCode(codes::hadamardProduct(code, code));
    newCode.puncture(columns);
    return invariantSup(newCode);  
}

std::string invariantConvecterHullHadSquareSimple(const codes::Lincode &code,
                                              const std::vector<size_t> &columns,
                                              std::function<std::string(const codes::Lincode &)>
                                              invariantSup) {
    codes::Lincode newCode(codes::hadamardProduct(code, code).hull());
    newCode.puncture(columns);
    return invariantSup(newCode);  
}

std::string invariantConvecterHadSquareSendrier(const codes::Lincode &code,
                                                const std::vector<size_t> &columns,
                                                std::function<std::string(const codes::Lincode &)>
                                                invariantSup) {
    if (columns.size() == 0) {
        return invariantSup(code);
    }
    std::vector<codes::Lincode> codesVec = {code, code};
    codesVec[0] = codes::hadamardProduct(codesVec[0], codesVec[0]);
    codesVec[1] = codesVec[0];
    codesVec[1].dual();
    std::string ans;
    for (size_t i = 0; i < codesVec.size(); ++i) {
        codesVec[i].puncture(columns);
        ans += invariantSup(codesVec[i]);
    }
    return ans;
}

} // namespace support

std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterSendrier(code, columns,
                                      codes::invariants::support::invariantWeightHullSupporter);
}

std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterSendrier(code, columns,
                              codes::invariants::support::invariantHullSizeSupporter);
}

std::string invariantSimpleHullSize(const codes::Lincode &code,
                                    const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterSimple(code, columns,
                                    codes::invariants::support::invariantSizeSupporter);
}

std::string invariantHadSquareSize(const codes::Lincode &code,
                                   const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterHadSquareSimple(code, columns,
                                             codes::invariants::support::invariantSizeSupporter);
}

std::string invariantWeightHadSquare(const codes::Lincode &code,
                                     const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterHadSquareSendrier(code, columns,
                                               codes::invariants::support::
                                               invariantWeightSupporter);
}

std::string invariantHullHadSquareSize(const codes::Lincode &code,
                                       const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterHullHadSquareSimple(code, columns,
                                                 codes::invariants::support::
                                                 invariantSizeSupporter);
}

std::string invariantWeightHullHadSquare(const codes::Lincode &code,
                                         const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterHullHadSquareSimple(code, columns,
                                                 codes::invariants::support::
                                                 invariantWeightSupporter);
}

std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterSendrier(code, columns,
                                      codes::invariants::support::invariantWeightBasisSupporter);
}

/*
std::string invariantMinRMSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterMinRM(code, columns,
                                   codes::invariants::support::invariantSizeSupporter);
}
*/

} //namespace invariants
} //namespace codes