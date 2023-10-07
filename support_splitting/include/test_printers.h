#include "support_splitting.h"

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
void printVV(std::vector<std::vector<size_t>> &vv);
void printVPV(std::vector<std::pair<std::vector<size_t>, std::vector<size_t>>> &v);

void printMSVD(const std::map<size_t, std::vector<SpectVectData>> &map, std::vector<size_t> used);
void printCC(std::vector<codes::Lincode> &cc);

void printD(const std::map<std::string, std::vector<std::pair<size_t, size_t>>> &d);

void printDD(const std::map<std::string, std::pair<std::set<size_t>, std::set<size_t>>> &d);

void printVSSAData(std::vector<SSAData> &v);

void printMCECD(std::map<std::string, ComplEqClData> &d);

} //namespace test_printers
} //namespace codes