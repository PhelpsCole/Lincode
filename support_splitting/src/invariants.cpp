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
        return invariantSize;
    case 3:
        return invariantWeight;
    case 4:
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
        return "invariantSize";
    case 3:
        return "invariantWeight";
    case 4:
        return "invariantWeightBasis";
    }
    return "invariantHullSize";
}

preprocType returnPreprocById(size_t id) {
    switch(id) {
    case 0:
        return preprocSimple;
    case 1:
        return preprocHull;
    case 2:
        return preprocHadPower;
    case 3:
        return preprocHadPowerHull;
    case 4:
        return preprocMaxRMSubMatrPqsigRM;
    }
    return preprocSimple;
}

std::string returnPreprocNameById(size_t id) {
    switch(id) {
    case 0:
        return "preprocSimple";
    case 1:
        return "preprocHull";
    case 2:
        return "preprocHadPower";
    case 3:
        return "preprocHadPowerHull";
    case 4:
        return "preprocMaxRMSubMatrPqsigRM";
    }
    return "preprocSimple";
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
    codes::Lincode newCode(code);
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

std::string invariantWeight(const codes::Lincode &code,
                                      const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterSimple(code, columns,
                                    codes::invariants::support::invariantWeightSupporter);
}

std::string invariantSize(const codes::Lincode &code,
                                    const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterSimple(code, columns,
                                    codes::invariants::support::invariantSizeSupporter);
}

std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns) {
    return codes::invariants::support::
           invariantConvecterSendrier(code, columns,
                                      codes::invariants::support::invariantWeightSupporter);
}

void preprocSimple(codes::Lincode &c) {}

void preprocHull(codes::Lincode &c) {
    c = c.hull();
}

void preprocHadPower(codes::Lincode &c) {
    c = codes::hadamardProduct(c, c);
}

void preprocHadPowerHull(codes::Lincode &c) {
    c = codes::hadamardProduct(c, c);
    c = c.hull();
}

// Maximize first block of pqsigRM matrix
void preprocMaxRMSubMatrPqsigRM(codes::Lincode &c) {
    std::vector<size_t> rmSizes = codes::rmSizes(c);
    std::vector<int> maxRMVector = codes::maxRMVector(rmSizes[0], rmSizes[1] - 2);
    for (size_t i = 0; i < maxRMVector.size(); ++i) {
        if (maxRMVector[i] == -1) {
            c.dual();
        } else {
            c = codes::hadPower(c, maxRMVector[i]);
        }
    }
}

} //namespace invariants
} //namespace codes