#include "matrix.h"
#include "algorithms.h"
#include <algorithm>

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
    std::vector<size_t> spectrum_basis() const;
    Lincode punctured(std::vector<size_t> &columns) const;
    Lincode punctured(size_t column) const;
    Lincode truncated(std::vector<size_t> &columns) const;

    std::vector<char> encode(std::vector<char> &);

    void basisView();
    void dual();
    void puncture(const std::vector<size_t> &columns);
    void puncture(size_t column);
    void truncate(std::vector<size_t> columns);

    void printCode() const;

private:
    size_t k;
    size_t n;
    std::vector<std::vector<char>> basis;
};

bool incorrect_basis(std::vector<std::vector<char>> &);
void addToBinVector(std::vector<char> &, size_t);

Lincode sum(const Lincode &, const Lincode &);
Lincode intersect(Lincode &, Lincode &);

Lincode mcEliece(const Lincode &);

} // namespace codes