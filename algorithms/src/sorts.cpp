#include "sorts.h"

namespace algorithms {
namespace sorts {

std::vector<unsigned long long> selectionSort(std::vector<unsigned long long> &vv,
                                              ullComparatorType comparator) {
    std::vector<unsigned long long> permVec(vv.size());
    std::iota(permVec.begin(), permVec.end(), 0);
    for (size_t i = 0; i < vv.size() - 1; ++i) {
        size_t ind = i;
        unsigned long long row = vv[ind];
        for (size_t j = i + 1; j < vv.size(); ++j) {
            if (!comparator(row, vv[j])) {
                ind = j;
                row = vv[ind];
            }
        }
        if (i != ind) {
            vv[ind] = vv[i];
            vv[i] = row;
            unsigned long long tmp = permVec[ind];
            permVec[ind] = permVec[i];
            permVec[i] = tmp;
        }
    }
    return permVec;
}

std::vector<unsigned long long> selectionSort(std::vector<std::vector<char>> &vv,
                                              charVecCompType comparator) {
    std::vector<unsigned long long> permVec(vv.size());
    std::iota(permVec.begin(), permVec.end(), 0);
    for (size_t i = 0; i < vv.size() - 1; ++i) {
        size_t ind = i;
        std::vector<char> row(vv[ind]);
        for (size_t j = i + 1; j < vv.size(); ++j) {
            if (!comparator(row, vv[j])) {
                ind = j;
                row = vv[ind];
            }
        }
        if (i != ind) {
            vv[ind] = vv[i];
            vv[i] = row;
            unsigned long long tmp = permVec[ind];
            permVec[ind] = permVec[i];
            permVec[i] = tmp;
        }
    }
    return permVec;
}

void plainSort(std::vector<std::vector<char>> &vv,
               std::function<bool(const std::vector<char> &,
                                  const std::vector<char> &)> comparator,
               std::vector<unsigned long long> &permVec,
               unsigned long long start,
               unsigned long long end){
    std::vector<std::vector<char>> result;
    for (size_t i = start; i < end; ++i) {
        result.push_back(vv[i]);
    }
    std::vector<unsigned long long> tempPermVec = selectionSort(result, comparator);
    std::vector<unsigned long long> delta(end - start);
    for (size_t i = 0; i < delta.size(); ++i) {
        delta[i] = permVec[start + i];
    }
    for (size_t i = 0; i < tempPermVec.size(); ++i) {
        vv[start + i] = result[i];
        permVec[start + i] = delta[tempPermVec[i]];
    }


}
void merge(std::vector<std::vector<char>> &vv,
           std::function<bool(const std::vector<char> &,
                              const std::vector<char> &)> comparator,
           std::vector<unsigned long long> &permVec,
           unsigned long long start,
           unsigned long long mid,
           unsigned long long end) {
    std::vector<std::vector<char>> result;
    size_t midTmp = mid, ind = 0;
    std::vector<unsigned long long> tmpPermVec(end - start);
    while(start != midTmp && mid != end){
        if (comparator(vv[mid], vv[start])) {
            tmpPermVec[ind] = permVec[mid];
            result.push_back(vv[mid++]);
            ++ind;
        }
        else {
            tmpPermVec[ind] = permVec[start];
            result.push_back(vv[start++]);
            ++ind;
        }

    }
    if (start == midTmp){
        while (mid != end) {
            tmpPermVec[ind] = permVec[mid];
            result.push_back(vv[mid++]);
            ++ind;
        }
    }
    else{
        while (start != midTmp) {
            tmpPermVec[ind] = permVec[start];
            result.push_back(vv[start++]);
            ++ind;
        }
    }
    while(ind--){
        vv[--end] = result[ind];
        permVec[end] = tmpPermVec[ind];
    }
}

void _mergeSort(std::vector<std::vector<char>> &vv,
                std::function<bool(const std::vector<char> &,
                                   const std::vector<char> &)> comparator,
                std::vector<unsigned long long> &permVec,
                unsigned long long start,
                unsigned long long end) {
        size_t MERGE_SIZE = 10;
        if (end - start < MERGE_SIZE) {
            plainSort(vv, comparator, permVec, start, end);
        }
        else {
            int mid = (start + end) / 2;
            _mergeSort(vv, comparator, permVec, start, mid);
            _mergeSort(vv, comparator, permVec, mid, end);
            merge(vv, comparator, permVec, start, mid, end);
        }
}

std::vector<unsigned long long> mergeSort(std::vector<std::vector<char>> &vv,
                                          std::function<bool(const std::vector<char> &,
                                                             const std::vector<char> &)>
                                          comparator) {
        std::vector<unsigned long long> permVec(vv.size());
        std::iota(permVec.begin(), permVec.end(), 0);
        _mergeSort(vv, comparator, permVec, 0, vv.size());
        return permVec;
}

} // namespace sorts
} // namespace algorithms