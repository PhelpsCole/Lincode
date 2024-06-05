#ifndef MINE_SORTS_H
#define MINE_SORTS_H

#include <vector>
#include <functional>
#include <algorithm>
// Strange moment that it needed for iota
#include <random>


namespace algorithms {
// Template sorts, which returns vector of permutations
namespace sorts {

typedef std::function<bool(const unsigned long long &, const unsigned long long &)> ullComparatorType;
typedef std::function<bool(const std::vector<char> &, const std::vector<char> &)> charVecCompType;

std::vector<unsigned long long> selectionSort(std::vector<unsigned long long> &vv,
                                              ullComparatorType comparator);
std::vector<unsigned long long> selectionSort(std::vector<std::vector<char>> &vv,
                                              charVecCompType comparator);
std::vector<unsigned long long> mergeSort(std::vector<std::vector<char>> &vv,
                                          std::function<bool(const std::vector<char> &,
                                                             const std::vector<char> &)>
                                          comparator);
void mergeSort(std::vector<unsigned long long> &vv,
               std::function<bool(const unsigned long long &,
                                  const unsigned long long &)> comparator);

} // namespace sorts
} // namespace algorithms

#endif // MINE_SORTS_H