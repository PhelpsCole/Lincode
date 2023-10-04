#include "support_functions.h"

namespace algorithms {

size_t hammingWeight(std::vector<char> v) {
	size_t weight = 0;
	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] != 0) {
			++weight;
		}
	}
	return weight;
}

size_t hammingDistance(std::vector<char> v, std::vector<char> v2) {
	if (v.size() != v2.size()) {
        throw std::invalid_argument("Vectors must have the same size");
    }
	size_t weight = 0;
	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] - v2[i] != 0) {
			++weight;
		}
	}
	return weight;
}


} // namespace algorithms