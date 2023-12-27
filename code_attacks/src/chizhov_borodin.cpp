#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

// Converts RM(r,m) -> RM(d,m)
codes::Lincode nodRM(const codes::Lincode &rm, int a, int b) {
    if (a == 0 && b == 1) {
        return rm;
    } else if (b != 0 && a * b <= 0) {
        if (b > 0) {
            a = 1 - a;
            b *= -1;
        }
        int q = -b / a;
        int s = q * a + b;
        codes::Lincode srm;
        if (s != 0) {
            srm = codes::hadPower(rm, s);
        }
        codes::Lincode qrm = codes::hadPower(rm, q);
        qrm.dual();
        qrm = codes::hadPower(qrm, a);
        if (s != 0) {
            codes::hadamardProduct(qrm, srm);
        }
        if (b > 0) {
            qrm.dual();
        }
        return qrm;
    } else {
        throw std::invalid_argument("Incorrect r and m sizes");
    }
}

} //namespace attackSupporters

// Return sigma^-1 by RM(r,m)^sigma
matrix::Matrix chizhov_borodin(codes::Lincode rm) {
    //Step 1: finding d = NOD(r, m-1)'
    std::vector<size_t> rmSizes = codes::rmSizes(rm);
    std::vector<int> ans = algorithms::extendedGcd(rmSizes[1] - 1, rmSizes[0]);
    size_t d = ans[0];
    codes::Lincode rm_d = attackSupporters::nodRM(rm, ans[1], ans[2]);

    codes::Lincode trivialRM;
    if (d == 1) {
        trivialRM = rm_d;
    } else {
        // Step 2: (d,m) => (d-1,m)' !!
        codes::Lincode rm_d2 = attackSupporters::rmReducer(rm_d);

        // Step 3: f((d, m)',(d-1,m)') => (1, m)'
        rm_d.dual();
        trivialRM = codes::hadamardProduct(rm_d, rm_d2);
        trivialRM.dual();
    }

    // Step 4: (1, m)' => (1, m) (get sigma)
    return codes::attackSupporters::simplePerm(trivialRM.toMatrix());
}

} // namespace codes