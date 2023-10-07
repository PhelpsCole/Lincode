#include "support_splitting.h"

namespace codes {
namespace support_ssa {

bool inSSDataFirst(const SSAData &d, size_t elem) {
    return std::find(d.dif1.begin(), d.dif1.end(), elem) != d.dif1.end() ||
           (d.used1.size() && std::find(d.used1.begin(), d.used1.end(), elem) != d.used1.end());
}

bool inSSDataSecond(const SSAData &d, size_t elem) {
    return std::find(d.dif2.begin(), d.dif2.end(), elem) != d.dif2.end() ||
           (d.used2.size() && std::find(d.used2.begin(), d.used2.end(), elem) != d.used2.end());
}

std::vector<SpectVectData>
spectPunctVector(const codes::Lincode &c,
                 std::vector<size_t> &used, size_t i,
                 std::vector<size_t> &dif, size_t set_size,
                 std::function<std::string(const codes::Lincode &)> invariant) {
    std::vector<SpectVectData> res;
    // Iterates by punct codes in eq class
    for (size_t j = 0; j < set_size; ++j) {
        std::vector<size_t> columns;
        columns.insert(columns.end(), used.begin(), used.end());
        columns.push_back(i);
        columns.push_back(dif[j]);
        codes::Lincode punct = c.punctured(columns);
        SpectVectData new_data;
        new_data.spectr = invariant(punct);
        new_data.dif = dif[j];
        res.push_back(new_data);
    }
    return res;
}

bool isBetterCandidate(const AnsParamSet &newCand, const AnsParamSet &cand) {
    if (newCand.cntFound > cand.cntFound) {
        return true;
    }
    return newCand.newEquivClasses.size() > cand.newEquivClasses.size();
}

} //namespace support_ssa
} //namespace codes