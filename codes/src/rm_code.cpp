#include <rm_code.h>

namespace codes {

RMCode::RMCode(size_t _r, size_t _m)
    : r(_r), m(_m), basis(generateRMCode(_r, _m)) {}

RMCode::RMCode(size_t _r, size_t _m, std::vector<std::vector<char>> &_basis)
    : r(_r), m(_m), basis(_basis) {}

matrix::Matrix RMCode::toMatrix() const {
    return matrix::Matrix(basis);
}

std::vector<std::vector<char>> &RMCode::getBasis() {
    return basis;
}

size_t RMCode::len() {
    return 1 << m;
}
size_t RMCode::size() {
    return basis.size();
}
size_t RMCode::min_dist() {
    return 1 << (m - r);
}

std::vector<std::vector<char>> generateRMCode(size_t r, size_t m) {
    size_t code_len = 1 << m;
    std::vector<char> a(code_len, 1);

    std::list<std::pair<int, std::vector<char>>> rows;
    rows.emplace_back(-1, a);

    if (r >= 1) {
        std::vector<std::vector<char>> monoms(m);
        for (unsigned int i = 0; i < m; ++i) {
            size_t r = 0;
            int b = 0;
            for (size_t j = 0; j < code_len; ++j) {
                if ((j >> (m - i - 1)) != r) {
                    ++r;
                    // Only contains 1 and 0
                    b = b ^ 1;
                }
                a[j] = static_cast<char>(b);
            }
            monoms[i] = a;
            rows.emplace_back(i, monoms[i]);
        }

        auto lStart = rows.begin();
        auto lEnd = rows.begin();
        std::advance(lEnd, rows.size() - 1);

        for (unsigned int i = 1; i < r; ++i) {
            for (auto e = std::next(lStart); e != lEnd; ++e) {
                for (unsigned int j = e->first + 1; j < m; ++j) {
                    std::vector<char> current_row(code_len);
                    for (size_t k = 0; k < code_len; ++k) {
                        current_row[k] = monoms[j][k] & e->second[k];
                    }
                    rows.emplace_back(j, current_row);
                }
            }
            for (unsigned int j = lEnd->first + 1; j < m; ++j) {
                std::vector<char> current_row(code_len);
                for (size_t k = 0; k < code_len; ++k) {
                    current_row[k] = monoms[j][k] & lEnd->second[k];
                }
                rows.emplace_back(j, current_row);
            }
            lStart = lEnd;
            lEnd = rows.begin();
            std::advance(lEnd, rows.size() - 1);
        }
    }

    std::vector<std::vector<char>> basis(rows.size());
    auto basis_iter = basis.begin();
    for (const auto& row : rows) {
        *basis_iter = row.second;
        ++basis_iter;
    }

    return basis;
}

} // namespace codes