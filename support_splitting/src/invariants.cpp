#include "invariants.h"

namespace codes {
namespace invariants {

std::string runInvariant(const codes::Lincode &code,
                         const std::vector<unsigned long long> &columns,
                         size_t id) {
    switch(id) {
    case 0:
        return invariantHullSize(code, columns);
    case 1:
        return invariantWeightHull(code, columns);
    case 2:
        return invariantSize(code, columns);
    case 3:
        return invariantWeight(code, columns);
    case 4:
        return invariantWeightBasis(code, columns);
    }
    return invariantHullSize(code, columns);
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

void runPreproc(codes::Lincode &c, size_t id) {
    switch(id) {
    case 0:
        return;
    case 1:
        return preprocHull(c);
    case 2:
        return preprocHadPower(c);
    case 3:
        return preprocHadPowerHull(c);
    case 4:
        return preprocMaxRMSubMatrPqsigRM(c);
    }
}

std::string returnPreprocNameById(size_t id) {
    switch(id) {
    case 0:
        return "No preproc";
    case 1:
        return "preprocHull";
    case 2:
        return "preprocHadPower";
    case 3:
        return "preprocHadPowerHull";
    case 4:
        return "preprocMaxRMSubMatrPqsigRM";
    }
    return "No preproc";
}

namespace support {
std::string invariantString(const std::vector<unsigned long long> &spectr) {
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
                                       const std::vector<unsigned long long> &columns,
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
                                     const std::vector<unsigned long long> &columns,
                                     std::function<std::string(const codes::Lincode &)>
                                     invariantSup) {
    codes::Lincode newCode(code);
    newCode.puncture(columns);
    return invariantSup(newCode);  
}

std::string invariantConvecterMinRM(const codes::Lincode &code,
                                    const std::vector<unsigned long long> &columns,
                                    std::function<std::string(const codes::Lincode &)>
                                    invariantSup) {
    codes::Lincode newCode(codes::minRM(code));
    newCode.puncture(columns);
    return invariantSup(newCode);  
}

} // namespace support

std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<unsigned long long> &columns) {
    return codes::invariants::support::
           invariantConvecterSendrier(code, columns,
                                      codes::invariants::support::invariantWeightHullSupporter);
}

std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<unsigned long long> &columns) {
    return codes::invariants::support::
           invariantConvecterSendrier(code, columns,
                                      codes::invariants::support::invariantHullSizeSupporter);
}

std::string invariantWeight(const codes::Lincode &code,
                                      const std::vector<unsigned long long> &columns) {
    return codes::invariants::support::
           invariantConvecterSimple(code, columns,
                                    codes::invariants::support::invariantWeightSupporter);
}

std::string invariantSize(const codes::Lincode &code,
                                    const std::vector<unsigned long long> &columns) {
    return codes::invariants::support::
           invariantConvecterSimple(code, columns,
                                    codes::invariants::support::invariantSizeSupporter);
}

std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<unsigned long long> &columns) {
    return codes::invariants::support::
           invariantConvecterSendrier(code, columns,
                                      codes::invariants::support::invariantWeightSupporter);
}

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
void preprocMaxRMSubMatrPqsigRM(codes::Lincode &pqsigRMcode) {
    maxRMSubMatrPqsigRM(pqsigRMcode);
}

} //namespace invariants
} //namespace codes