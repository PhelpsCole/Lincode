#include "support_splitting.h"

namespace codes {
namespace support_ssa {

struct SSAData {
    std::vector<size_t> dif1;
    std::vector<size_t> used1;
    std::vector<size_t> dif2;
    std::vector<size_t> used2;
};

struct SpectVectData {
    std::string spectr;
    size_t dif;
};

struct ComplEqClData {
    std::set<size_t> dif_set1;
    std::set<size_t> dif_set2;
    size_t new_used1;
    size_t new_used2;
};

struct AnsParamSet {
    std::vector<size_t> ans;
    size_t cntFound;
    std::vector<SSAData> newEquivClasses;
};

bool inSSDataFirst(const SSAData &d, size_t elem);
bool inSSDataSecond(const SSAData &d, size_t elem);

bool isBetterCandidate(const AnsParamSet &, const AnsParamSet &);
bool ansHaveZeroes(const std::vector<size_t> &ans);

std::vector<SpectVectData>
spectPunctVector(const codes::Lincode &c, std::vector<size_t> &used,
                 size_t i, std::vector<size_t> &dif, size_t set_size,
                 std::function<std::string(const codes::Lincode &)> invariant);

} //namespace support_ssa
} //namespace codes