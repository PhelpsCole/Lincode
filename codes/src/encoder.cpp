#include "encoder.h"

namespace codes {

Encoder::Encoder(const Lincode &_c) : c(_c), cMatr(_c.toMatrix()) {
    weightVec = std::make_pair(std::vector<char>(c.size()), 0);
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

size_t Encoder::stageWeight() const {
    return weightVec.second;
}

codes::Lincode Encoder::code() const {
    return c;
}

} //namespace codes