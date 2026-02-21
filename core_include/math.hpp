#ifndef CORE_INCLUDE_MATH_HPP
#define CORE_INCLUDE_MATH_HPP

#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"

namespace ademma_core::math
{
ClassicalAdemPolynomial admissify_classical_adem_monomial(const ClassicalAdemMonomial& aMonomial);
}

#endif // CORE_INCLUDE_MATH_HPP

