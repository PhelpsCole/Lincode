#ifndef LINCODE_ENCODER_H
#define LINCODE_ENCODER_H

#include "algorithms.h"
#include "linear_code.h"

namespace codes {

class Encoder {
public:
    //used as default and for zero codes
    Encoder(const Lincode &);

    std::vector<char> next(size_t = 1);
    std::vector<char> current() const;
    std::vector<char> stage() const;
    bool isEnd() const;
    unsigned long long stageWeight() const;
    codes::Lincode code() const;

private:
    codes::Lincode c;
    matrix::Matrix cMatr;
    std::pair<std::vector<char>, unsigned long long> weightVec;
};

} // namespace codes

#endif // LINCODE_ENCODER_H