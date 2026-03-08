#ifndef CORE_INCLUDE_C_MOTIVIC_ADEM_POLYNOMIAL
#define CORE_INCLUDE_C_MOTIVIC_ADEM_POLYNOMIAL

#include "c_motivic_adem_monomial.hpp"

#include <vector>

namespace ademma_core
{
typedef std::vector<CMotivicAdemMonomial> CMotivicAdemPolynomial;

std::string CMotivicAdemPolynomial_ToString(const CMotivicAdemPolynomial& aValue);

CMotivicAdemPolynomial CMotivicAdemPolynomial_FromString(ParsingInfo& aParsingInfo);

void CMotivicAdemPolynomial_EliminateAllSq0Factors(CMotivicAdemPolynomial& aPolynomial);

void CMotivicAdemPolynomial_ShoveTauLeft(CMotivicAdemPolynomial& aPolynomial);

void CMotivicAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors_AssumeTauLeft(CMotivicAdemPolynomial& aPolynomial);

bool CMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(const CMotivicAdemPolynomial& aPolynomial);

CMotivicAdemPolynomial CMotivicAdemPolynomial_MultiplyPolynomial(const CMotivicAdemPolynomial& aLeft, const CMotivicAdemPolynomial& aRight);

void CMotivicAdemPolynomial_MultiplyLeftMonomial(const CMotivicAdemMonomial& aLeft, CMotivicAdemPolynomial& aRight);

void CMotivicAdemPolynomial_MultiplyRightMonomial(CMotivicAdemPolynomial& aLeft, const CMotivicAdemMonomial& aRight);

void CMotivicAdemPolynomial_ReplaceTermWithPolynomial(CMotivicAdemPolynomial& aPolynomial, size_t aTermIndex, const CMotivicAdemPolynomial& aReplacementPolynomial);
}

#endif // CORE_INCLUDE_C_MOTIVIC_ADEM_POLYNOMIAL

