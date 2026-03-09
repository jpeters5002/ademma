#ifndef CORE_INCLUDE_C_MOTIVIC_ADEM_MATH_HPP
#define CORE_INCLUDE_C_MOTIVIC_ADEM_MATH_HPP

#include "c_motivic_adem_monomial.hpp"
#include "c_motivic_adem_polynomial.hpp"

namespace ademma_core { namespace c_motivic_adem_math
{
CMotivicAdemPolynomial admissify_c_motivic_adem_polynomial(const CMotivicAdemPolynomial& aPolynomial);

CMotivicAdemPolynomial admissify_c_motivic_adem_monomial(const CMotivicAdemMonomial& aMonomial);
}}

#endif // CORE_INCLUDE_C_MOTIVIC_ADEM_MATH_HPP

