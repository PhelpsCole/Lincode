#ifndef LINCODE_CODE_H
#define LINCODE_CODE_H

#include "matrix.h"
#include "algorithms.h"
//#include <algorithm>

namespace codes {

class Lincode {
public:
    //used as default and for zero codes
    Lincode(size_t = 0, size_t = 0);
    Lincode(const std::vector<std::vector<char>> &, bool check_basis = false);
    Lincode(const Lincode &, bool check_basis = false);
    Lincode(const matrix::Matrix &, bool check_basis = false);
    Lincode(std::string &,  char tabs = '\0', char sep = '\n', bool check_basis = false);

    size_t size() const;
    size_t len() const;
    size_t min_dist() const;
    matrix::Matrix toMatrix() const;
    const std::vector<std::vector<char>> &getBasis() const;
    Lincode hull() const;
    std::vector<size_t> spectrum() const;
    std::string stringSpectrum() const;
    std::vector<size_t> spectrum_basis() const;
    Lincode punctured(const std::vector<size_t> &columns) const;
    Lincode punctured(size_t column) const;
    Lincode truncated(std::vector<size_t> &columns) const;

    std::vector<char> encode(std::vector<char> &);

    void basisView();
    void dual();
    void puncture(const std::vector<size_t> &columns);
    void puncture(size_t column);
    void truncate(std::vector<size_t> columns);

    void printCodeSizes() const;
    void printCode() const;
    void printVisualCode(size_t blocks_num = 1) const;

private:
    size_t k;
    size_t n;
    std::vector<std::vector<char>> basis;
};

Lincode sum(const Lincode &, const Lincode &);
Lincode intersect(Lincode &, Lincode &);
Lincode hadamardProduct(const Lincode &, const Lincode &);
Lincode hadPower(const Lincode &c, size_t power);

Lincode mcEliece(const Lincode &);

} // namespace codes

#endif // LINCODE_CODE_H