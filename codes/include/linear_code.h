#include "matrix.h"
#include "algorithms.h"

namespace codes {

class Lincode {
public:
    Lincode(size_t, size_t);
    //Lincode(size_t, size_t, std::vector<char> &);
    Lincode(std::vector<std::vector<char>> &);
    Lincode(Lincode &);
    Lincode(matrix::Matrix &);
    Lincode(const Lincode &) = default;

    size_t len() const;
    size_t size() const;
    size_t min_dist() const;
    std::vector<std::vector<char>> &getBasis();

    Lincode dual();

    void printCode() const;

private:
    size_t k;
    size_t n;
    std::vector<std::vector<char>> basis;
};


} // namespace codes