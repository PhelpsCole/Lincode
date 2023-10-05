#include "matrix.h"
#include "algorithms.h"

namespace codes {

class Lincode {
public:
    //to delete
    Lincode(size_t, size_t);
    Lincode(const std::vector<std::vector<char>> &, bool check_basis = false);
    Lincode(const Lincode &, bool check_basis = false);
    Lincode(const matrix::Matrix &, bool check_basis = false);

    size_t size() const;
    size_t len() const;
    size_t min_dist() const;
    matrix::Matrix toMatrix() const;
    const std::vector<std::vector<char>> &getBasis() const;
    Lincode hull() const;
    std::vector<size_t> spectrum() const;

    std::vector<char> encode(std::vector<char> &);

    void dual();

    void printCode() const;

private:
    size_t k;
    size_t n;
    std::vector<std::vector<char>> basis;
};

bool incorrect_basis(std::vector<std::vector<char>> &);

Lincode sum(const Lincode &, const Lincode &);
Lincode intersect(Lincode &, Lincode &);

void addToBinVector(std::vector<char> &, size_t);

} // namespace codes