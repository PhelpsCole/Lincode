#include "support_splitting.h"
#include "ssa_supports.h"

namespace codes {
namespace test_printers {

using namespace codes::support_ssa;

template<typename T>
void printV(const std::vector<T> &v) {
    for (size_t j = 0; j < v.size(); ++j) {
        std::cout << v[j] << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void printVInd(const std::vector<T> &v) {
    for (size_t j = 0; j < v.size(); ++j) {
        std::cout << "(" << j + 1 << ", " << v[j] << ") ";
    }
    std::cout << std::endl;
}

void printVV(const std::vector<std::vector<size_t>> &vv);
void printVPV(const std::vector<std::pair<std::vector<size_t>, std::vector<size_t>>> &v);

void printMSVD(const std::map<size_t, std::vector<SpectVectData>> &map, std::vector<size_t> used);

void printCC(const std::vector<codes::Lincode> &cc);

void printD(const std::map<std::string, std::vector<std::pair<size_t, size_t>>> &d);
void printDD(const std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> &d);

void printVSSAData(const std::vector<SSAData> &v);

void printMCECD(const std::map<std::string, ComplEqClData> &d);

void printAnsCandidate(const AnsParamSet &);

} //namespace test_printers
} //namespace codes