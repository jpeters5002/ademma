#ifndef CORE_INCLUDE_CLASSICAL_ADEM_MONOMIAL
#define CORE_INCLUDE_CLASSICAL_ADEM_MONOMIAL

#include "steenrod_square.hpp"

#include <vector>
#include <string>

namespace ademma_core
{
typedef std::vector<SteenrodSquareDegree> ClassicalAdemMonomial;
std::string ClassicalAdemMonomial_ToString(const ClassicalAdemMonomial& aValue);
}

#endif // CORE_INCLUDE_CLASSICAL_ADEM_MONOMIAL

