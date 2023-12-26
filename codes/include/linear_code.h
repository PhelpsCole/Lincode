#ifndef LINCODE_CODE_H
#define LINCODE_CODE_H

#include "matrix.h"
#include "algorithms.h"

namespace codes {

class Lincode {
public:
    //used as default and for zero codes
    Lincode(unsigned long long = 0, unsigned long long = 0);
    Lincode(const std::vector<std::vector<char>> &, bool check_basis = false);
    Lincode(const Lincode &, bool check_basis = false);
    Lincode(const matrix::Matrix &, bool check_basis = false);
    Lincode(std::string &,  char tabs = '\0', char sep = '\n', bool check_basis = false);

    unsigned long long size() const;
    unsigned long long len() const;
    unsigned long long min_dist() const;
    matrix::Matrix toMatrix() const;
    const std::vector<std::vector<char>> &getBasis() const;
    Lincode hull() const;
    std::vector<unsigned long long> spectrum() const;
    std::string stringSpectrum() const;
    std::vector<unsigned long long> spectrum_basis() const;
    Lincode punctured(const std::vector<unsigned long long> &columns) const;
    Lincode punctured(unsigned long long column) const;
    Lincode truncated(std::vector<unsigned long long> &columns) const;

    std::vector<char> encode(std::vector<char> &);

    void basisView();
    void dual();
    void puncture(const std::vector<unsigned long long> &columns);
    void puncture(unsigned long long column);
    void truncate(std::vector<unsigned long long> columns);

    void printCodeSizes(const std::string &filename = "NoName") const;
    void printCode(const std::string &filename = "NoName", char sep = ' ', std::string end = "\n") const;
    void printVisualCode(unsigned long long blocks_num = 1,
                         const std::string &filename = "NoName",
                         std::string end = "\n") const;

private:
    unsigned long long k;
    unsigned long long n;
    std::vector<std::vector<char>> basis;
};

Lincode sum(const Lincode &, const Lincode &);
Lincode intersect(Lincode &, Lincode &);
Lincode hadamardProduct(const Lincode &, const Lincode &);
Lincode hadPower(const Lincode &c, size_t power);
bool linearDependence(const Lincode &, const Lincode &);
bool isEquivalent(Lincode, Lincode);

Lincode mcEliece(const Lincode &);

} // namespace codes

#endif // LINCODE_CODE_H