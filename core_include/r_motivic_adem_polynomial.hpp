#ifndef CORE_INCLUDE_R_MOTIVIC_ADEM_POLYNOMIAL
#define CORE_INCLUDE_R_MOTIVIC_ADEM_POLYNOMIAL

#include "r_motivic_adem_monomial.hpp"

#include <vector>

namespace ademma_core
{
typedef std::vector<RMotivicAdemMonomial> RMotivicAdemPolynomial;

std::string RMotivicAdemPolynomial_ToString(const RMotivicAdemPolynomial& aValue);

RMotivicAdemPolynomial RMotivicAdemPolynomial_FromString(ParsingInfo& aParsingInfo);

void RMotivicAdemPolynomial_EliminateAllSq0Factors(RMotivicAdemPolynomial& aPolynomial);

void RMotivicAdemPolynomial_ShoveRhoLeft(RMotivicAdemPolynomial& aPolynomial);

void RMotivicAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors_AssumeRhoLeft(RMotivicAdemPolynomial& aPolynomial);

bool RMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(const RMotivicAdemPolynomial& aPolynomial);

RMotivicAdemPolynomial RMotivicAdemPolynomial_MultiplyPolynomial(const RMotivicAdemPolynomial& aLeft, const RMotivicAdemPolynomial& aRight);

void RMotivicAdemPolynomial_AddRightPolynomial(RMotivicAdemPolynomial& aLeft, const RMotivicAdemPolynomial& aRight);

void RMotivicAdemPolynomial_MultiplyLeftMonomial(const RMotivicAdemMonomial& aLeft, RMotivicAdemPolynomial& aRight);

void RMotivicAdemPolynomial_MultiplyRightMonomial(RMotivicAdemPolynomial& aLeft, const RMotivicAdemMonomial& aRight);

void RMotivicAdemPolynomial_ReplaceTermWithPolynomial(RMotivicAdemPolynomial& aPolynomial, size_t aTermIndex, const RMotivicAdemPolynomial& aReplacementPolynomial);
}

#endif // CORE_INCLUDE_R_MOTIVIC_ADEM_POLYNOMIAL

