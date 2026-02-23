#ifndef CORE_INCLUDE_R_MOTIVIC_ADEM_POLYNOMIAL
#define CORE_INCLUDE_R_MOTIVIC_ADEM_POLYNOMIAL

#include "steenrod_square.hpp"
#include "classical_adem_monomial.hpp"

#include <vector>

namespace ademma_core
{
typedef std::vector<ClassicalAdemMonomial> ClassicalAdemPolynomial;

std::string ClassicalAdemPolynomial_ToString(const ClassicalAdemPolynomial& aValue);

void ClassicalAdemPolynomial_EliminateAllSq0Factors(ClassicalAdemPolynomial& aPolynomial);

void ClassicalAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors(ClassicalAdemPolynomial& aPolynomial);

bool ClassicalAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(const ClassicalAdemPolynomial& aPolynomial);

ClassicalAdemPolynomial ClassicalAdemPolynomial_MultiplyPolynomial(const ClassicalAdemPolynomial& aLeft, const ClassicalAdemPolynomial& aRight);

void ClassicalAdemPolynomial_MultiplyLeftMonomial(const ClassicalAdemMonomial& aLeft, ClassicalAdemPolynomial& aRight);

void ClassicalAdemPolynomial_MultiplyRightMonomial(ClassicalAdemPolynomial& aLeft, const ClassicalAdemMonomial& aRight);

void ClassicalAdemPolynomial_ReplaceTermWithPolynomial(ClassicalAdemPolynomial& aPolynomial, size_t aTermIndex, const ClassicalAdemPolynomial& aReplacementPolynomial);
}

#endif // CORE_INCLUDE_R_MOTIVIC_ADEM_POLYNOMIAL

