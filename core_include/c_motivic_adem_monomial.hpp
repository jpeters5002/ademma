#ifndef CORE_INCLUDE_C_MOTIVIC_ADEM_MONOMIAL
#define CORE_INCLUDE_C_MOTIVIC_ADEM_MONOMIAL

#include "steenrod_square.hpp"
#include "parsing_info.hpp"

#include <vector>
#include <string>

namespace ademma_core
{
// FACTOR

typedef SteenrodSquareDegree CMotivicAdemMonomialFactor;

static constexpr const CMotivicAdemMonomialFactor cCMotivicAdemMonomialFactor_ERROR_VALUE = cSteenrodSquareDegree_ERROR_VALUE;

// msb
static constexpr const CMotivicAdemMonomialFactor cCMotivicAdemMonomialFactor_IS_TAU_BIT = 1 << (8 * sizeof(CMotivicAdemMonomialFactor) - 1);
enum class CMotivicAdemMonomialFactor_Type : CMotivicAdemMonomialFactor
{
    cSteenrodSquareDegree = 0,
    cTau,

    cCOUNT,
    cNONE = cCOUNT,
};

CMotivicAdemMonomialFactor_Type CMotivicAdemMonomialFactor_GetType(CMotivicAdemMonomialFactor aValue);

CMotivicAdemMonomialFactor CMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(SteenrodSquareDegree aValue);

CMotivicAdemMonomialFactor CMotivicAdemMonomialFactor_CreateTau(int aPower);

std::string CMotivicAdemMonomialFactor_ToString(CMotivicAdemMonomialFactor aValue);

CMotivicAdemMonomialFactor CMotivicAdemMonomialFactor_FromString(ParsingInfo& aParsingInfo);

int CMotivicAdemMonomialFactor_GetPower_AssumeTau(CMotivicAdemMonomialFactor aValue);

bool CMotivicAdemMonomialFactor_IsPairAdmissible(CMotivicAdemMonomialFactor aLeft, CMotivicAdemMonomialFactor aRight);

// MONOMIAL

typedef std::vector<CMotivicAdemMonomialFactor> CMotivicAdemMonomial;

std::string CMotivicAdemMonomial_ToString(const CMotivicAdemMonomial& aValue);

CMotivicAdemMonomial CMotivicAdemMonomial_FromString(ParsingInfo& aParsingInfo);

void CMotivicAdemMonomial_EliminateAllSq0Factors(CMotivicAdemMonomial& aMonomial);

void CMotivicAdemMonomial_ShoveTauLeft(CMotivicAdemMonomial& aMonomial);

bool CMotivicAdemMonomial_IsEqualInForm(const CMotivicAdemMonomial& aLeft, const CMotivicAdemMonomial& aRight);

bool CMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(const CMotivicAdemMonomial& aMonomial);

CMotivicAdemMonomial CMotivicAdemMonomial_Multiply(const CMotivicAdemMonomial& aLeft, const CMotivicAdemMonomial& aRight);
}

#endif // CORE_INCLUDE_C_MOTIVIC_ADEM_MONOMIAL

