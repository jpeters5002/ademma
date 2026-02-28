#ifndef TEST_INCLUDE_TESTING_HELPER_FUNCTIONS_HPP
#define TEST_INCLUDE_TESTING_HELPER_FUNCTIONS_HPP

#include "r_motivic_adem_polynomial.hpp"
#include "classical_adem_polynomial.hpp"

namespace testing
{
ademma_core::ClassicalAdemPolynomial classical_polynomial_from_r_motivic_polynomial(const ademma_core::RMotivicAdemPolynomial& aRMPolynomial);
int two_to_power(int n);
}

#endif // TEST_INCLUDE_TESTING_HELPER_FUNCTIONS_HPP

