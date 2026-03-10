#ifndef TEST_INCLUDE_TESTING_HELPER_FUNCTIONS_HPP
#define TEST_INCLUDE_TESTING_HELPER_FUNCTIONS_HPP

#include "c_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "classical_adem_polynomial.hpp"

namespace testing
{
ademma_core::ClassicalAdemPolynomial classical_polynomial_from_r_motivic_polynomial(const ademma_core::RMotivicAdemPolynomial& aRMPolynomial, int* aNumTermsWithTau = nullptr, int* aNumTermsWithRho = nullptr);

ademma_core::ClassicalAdemPolynomial classical_polynomial_from_c_motivic_polynomial(const ademma_core::CMotivicAdemPolynomial& aCMPolynomial, int* aNumTermsWithTau = nullptr);

int two_to_power(int n);
}

#endif // TEST_INCLUDE_TESTING_HELPER_FUNCTIONS_HPP

