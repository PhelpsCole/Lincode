#ifndef CODE_ATTACKS_H
#define CODE_ATTACKS_H

#include "sorts.h"
#include "codes.h"

namespace codes {
namespace attackSupporters {

codes::Lincode nodRM(const codes::Lincode &rm, int a, int b);

} //namespace attackSupporters

matrix::Matrix chizhov_borodin(codes::Lincode);

} // namespace codes

#endif // CODE_ATTACKS_H