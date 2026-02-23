#ifndef CORE_INCLUDE_R_MOTIVIC_ADEM_MONOMIAL
#define CORE_INCLUDE_R_MOTIVIC_ADEM_MONOMIAL

#include "steenrod_square.hpp"
#include "parsing_info.hpp"

#include <vector>
#include <string>

namespace ademma_core
{
// FACTOR

typedef SteenrodSquareDegree RMotivicAdemMonomialFactor;

static constexpr const RMotivicAdemMonomialFactor cRMotivicAdemMonomialFactor_ERROR_VALUE = cSteenrodSquareDegree_ERROR_VALUE;

// msb
static constexpr const RMotivicAdemMonomialFactor cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT = 1 << (8 * sizeof(RMotivicAdemMonomialFactor) - 1);
// msb >> 1 (only applicable if IS_RHO_OR_TAU_BIT is set)
static constexpr const RMotivicAdemMonomialFactor cRMotivicAdemMonomialFactor_IS_TAU_BIT = 1 << (8 * sizeof(RMotivicAdemMonomialFactor) - 2);
enum class RMotivicAdemMonomialFactor_Type : RMotivicAdemMonomialFactor
{
    cSteenrodSquareDegree = 0,
    cTau,
    cRho,

    cCOUNT,
    cNONE = cCOUNT,
};

RMotivicAdemMonomialFactor_Type RMotivicAdemMonomialFactor_GetType(RMotivicAdemMonomialFactor aValue);

std::string RMotivicAdemMonomialFactor_ToString(RMotivicAdemMonomialFactor aValue);

RMotivicAdemMonomialFactor RMotivicAdemMonomialFactor_FromString(ParsingInfo& aParsingInfo);

int RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(RMotivicAdemMonomialFactor aValue);

bool RMotivicAdemMonomialFactor_IsPairAdmissible(RMotivicAdemMonomialFactor aLeft, RMotivicAdemMonomialFactor aRight);

// MONOMIAL

typedef std::vector<RMotivicAdemMonomialFactor> RMotivicAdemMonomial;

std::string RMotivicAdemMonomial_ToString(const RMotivicAdemMonomial& aValue);

RMotivicAdemMonomial RMotivicAdemMonomial_FromString(ParsingInfo& aParsingInfo);

void RMotivicAdemMonomial_EliminateAllPower0Taus(RMotivicAdemMonomial& aMonomial);

void RMotivicAdemMonomial_EliminateAllSq0Factors(RMotivicAdemMonomial& aMonomial);

void RMotivicAdemMonomial_ShoveRhoLeft(RMotivicAdemMonomial& aMonomial);

bool RMotivicAdemMonomial_IsEqualInForm(const RMotivicAdemMonomial& aLeft, const RMotivicAdemMonomial& aRight);

bool RMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(const RMotivicAdemMonomial& aMonomial);

RMotivicAdemMonomial RMotivicAdemMonomial_Multiply(const RMotivicAdemMonomial& aLeft, const RMotivicAdemMonomial& aRight);
}

#endif // CORE_INCLUDE_R_MOTIVIC_ADEM_MONOMIAL

