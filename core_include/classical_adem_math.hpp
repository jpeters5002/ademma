#ifndef CORE_INCLUDE_CLASSICAL_ADEM_MATH_HPP
#define CORE_INCLUDE_CLASSICAL_ADEM_MATH_HPP

#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"

namespace ademma_core { namespace classical_adem_math
{
ClassicalAdemPolynomial admissify_classical_adem_polynomial(const ClassicalAdemPolynomial& aPolynomial);

ClassicalAdemPolynomial admissify_classical_adem_monomial(const ClassicalAdemMonomial& aMonomial);
}}

#endif // CORE_INCLUDE_CLASSICAL_ADEM_MATH_HPP

