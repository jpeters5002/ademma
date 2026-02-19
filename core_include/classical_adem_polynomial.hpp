#ifndef CORE_INCLUDE_CLASSICAL_ADEM_POLYNOMIAL
#define CORE_INCLUDE_CLASSICAL_ADEM_POLYNOMIAL

#include "steenrod_square.hpp"
#include "classical_adem_monomial.hpp"

#include <vector>

namespace ademma_core
{
typedef std::vector<ClassicalAdemMonomial> ClassicalAdemPolynomial;
std::string ClassicalAdemPolynomial_ToString(const ClassicalAdemPolynomial& aValue);
}

#endif // CORE_INCLUDE_CLASSICAL_ADEM_POLYNOMIAL

