#include <rm_code.h>

namespace codes {

RMCode::RMCode(size_t _r, size_t _m)
    : r(_r), m(_m), basis(generateRMCode(_r, _m)) {}

RMCode::RMCode(size_t _r, size_t _m, std::vector<std::vector<char>> &_basis)
    : r(_r), m(_m), basis(_basis) {}

matrix::Matrix RMCode::toMatrix() const {
    return matrix::Matrix(basis);
}

const std::vector<std::vector<char>> &RMCode::getBasis() {
    return basis;
}

unsigned long long RMCode::len() {
    return 1 << m;
}
unsigned long long RMCode::size() {
    return basis.size();
}
unsigned long long RMCode::min_dist() {
    return 1 << (m - r);
}

std::vector<std::vector<char>> generateRMCode(size_t r, size_t m) {
    unsigned long long code_len = 1 << m;
    std::vector<char> a(code_len, 1);

    std::list<std::pair<int, std::vector<char>>> rows;
    rows.emplace_back(-1, a);

    if (r >= 1) {
        std::vector<std::vector<char>> monoms(m);
        for (unsigned int i = 0; i < m; ++i) {
            size_t r = 0;
            int b = 0;
            for (unsigned long long j = 0; j < code_len; ++j) {
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
                    for (unsigned long long k = 0; k < code_len; ++k) {
                        current_row[k] = monoms[j][k] & e->second[k];
                    }
                    rows.emplace_back(j, current_row);
                }
            }
            for (unsigned int j = lEnd->first + 1; j < m; ++j) {
                std::vector<char> current_row(code_len);
                for (unsigned long long k = 0; k < code_len; ++k) {
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

// Returns (r, m) if this is rm-code and (0, 0) if not
std::vector<size_t> rmSizes(const codes::Lincode &c) {
    std::vector<size_t> res(2);
    size_t m = c.len();
    while (m != 1) {
        if (m % 2 == 1) {
            return std::vector<size_t>(2);
        }
        m >>= 1;
        ++res[1];
    }
    long long k = c.size() - 1;
    size_t tmp = res[1];
    while (k > 0) {
        k -= tmp;
        ++res[0];
        tmp *= (res[1] - res[0]);
        tmp /= (res[0] + 1);
    }
    //if (k < 0) {
    //    return std::vector<size_t>(2);
    //}
    // возвращаю максимальный r
    return res;
}

unsigned long long codeSizeFromRM(size_t r, size_t m) {
    unsigned long long k = 1;
    unsigned long long tmp = k, cnt = r, delta = m;
    while (cnt-- != 0) {
        tmp *= delta--;
        tmp /= r - cnt;
        k += tmp;
    }
    return k;
}

// Vector contatins -1 (dual) and q (q-hadamard product)
std::vector<int> minRMVector(size_t r, size_t m) {
    std::vector<int> res;
    if (r == 0 || r == 1 || m - r == 1) {
        return res;
    }
    if (2*r >= m) {
        res.push_back(-1);
        r = m - r - 1;
    }
    while (r != 1 && m % r != 1) {
        size_t q = m / r;
        if (m % r == 0) {
            --q;
        }
        if (q != 1) {
            res.push_back(q);
        }
        r = m - q*r - 1;
        res.push_back(-1);
    }
    return res;
}

// Vector contatins -1 (dual) and q (q-hadamard product)
std::vector<int> maxRMVector(size_t r, size_t m) {
    std::vector<int> res;
    if (r == 0 || m - r == 1) {
        return res;
    }
    if (r == 1) {
        res.push_back(m / r - 1);
        return res;
    }
    if (2*r >= m) {
        res.push_back(-1);
        r = m - r - 1;
    }
    while (r != 1 && m % r != 1) {
        size_t q = m / r;
        if (m % r == 0) {
            --q;
        }
        if (q != 1) {
            res.push_back(q);
        }
        r = m - q*r - 1;
        res.push_back(-1);
    }
    if (res.size() == 0 && m / r == 1) {
        return res;
    }
    res.push_back(m / r);
    //if (m % r == 0) {
    //    --res[res.size() - 1];
    //}
    return res;
}

codes::Lincode minRM(const codes::Lincode &code) {
    std::vector<size_t> rmSizes = codes::rmSizes(code);
    size_t r = rmSizes[0], m = rmSizes[1];
    codes::Lincode newCode(code);
    std::vector<int> convVector = minRMVector(r, m);
    for (size_t i = 0; i < convVector.size(); ++i) {
        if (convVector[i] == -1) {
            newCode.dual();
        } else {
            newCode = codes::hadPower(newCode, convVector[i]);
        }
    }
    return newCode;  
}

} // namespace codes