#ifndef CUSTOM_PERMUTATION_H
#define CUSTOM_PERMUTATION_H

#include <vector>
#include <matrix.h>

namespace permutation {

// Numeration from zero. Example: {3, 0, 1, 2, 4}
class Permutation {
public:
    Permutation(unsigned long long len);
    Permutation(const std::vector<unsigned long long> &vec);
    Permutation(const matrix::Matrix &);
    Permutation(const Permutation &);
    ~Permutation() = default;

    Permutation &operator=(const Permutation &) = default;
    Permutation operator*(const Permutation &other) const;
    void operator*=(const Permutation &other);
    void reverse();
    void permute(std::vector<unsigned long long> &inp) const;
    void permute(matrix::Matrix &matr); // M * P

    unsigned long long len() const;
    std::vector<unsigned long long> vec() const;
    matrix::Matrix matrix();

private:
    void _setMatrix();
    void _setVector();
    void _clearMatrix();
    unsigned long long _len;
    std::vector<unsigned long long> _vec;
    bool _definedMatrix;
    matrix::Matrix _matr;
};

Permutation generateRandomPermutation(unsigned long long size);
Permutation generateRandomPermutationOfP(unsigned long long size, unsigned long long p);

} // namespace permutation

#endif // CUSTOM_PERMUTATION_H