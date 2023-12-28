#include "encoder.h"

namespace codes {

Encoder::Encoder(const Lincode &_c) : cMatr(_c.toMatrix()) {
    weightVec = std::make_pair(std::vector<char>(cMatr.rows()), 0);
}

std::vector<char> Encoder::next(size_t steps) {
    algorithms::addToBinWeightVector(weightVec, steps);
    return cMatr.multiplyVectorByMatrix(weightVec.first);
}

std::vector<char> Encoder::current() const {
    return cMatr.multiplyVectorByMatrix(weightVec.first);
}

std::vector<char> Encoder::stage() const {
    return weightVec.first;
}

bool Encoder::isEnd() const {
    return weightVec.second == weightVec.first.size();
}

unsigned long long Encoder::stageWeight() const {
    return weightVec.second;
}

codes::Lincode Encoder::code() const {
    return cMatr;
}

} //namespace codes