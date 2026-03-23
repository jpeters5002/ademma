#ifndef CORE_INCLUDE_ARBITRARY_CALCULATION_INPUT_HPP
#define CORE_INCLUDE_ARBITRARY_CALCULATION_INPUT_HPP

#include <vector>

#include "setting.hpp"
#include "parsing_info.hpp"

#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"
#include "c_motivic_adem_monomial.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_monomial.hpp"
#include "r_motivic_adem_polynomial.hpp"

// thank you c++ for butchering unions

namespace ademma_core
{
enum class ACITerm_Type
{
    cADD,
    cMULTIPLY,
    cSUBACI, // implies ACITerm::mData is type ArbitraryCalculationInput*
    cPOLYNOMIAL, // implies ACITerm::mData is type <Setting>AdemPolynomial*
    cMONOMIAL, // implies ACITerm::mData is type <Setting>AdemMonomial*

    cNONE,
};
struct ArbitraryCalculationInput;
union ACITermData
{
    ArbitraryCalculationInput* mSubACI;
    ClassicalAdemPolynomial* mClassicalAdemPolynomial;
    CMotivicAdemPolynomial* mCMotivicAdemPolynomial;
    RMotivicAdemPolynomial* mRMotivicAdemPolynomial;
    ClassicalAdemMonomial* mClassicalAdemMonomial;
    CMotivicAdemMonomial* mCMotivicAdemMonomial;
    RMotivicAdemMonomial* mRMotivicAdemMonomial;
};
struct ACITerm
{
    ACITerm_Type mType;
    Setting_Type mSetting;
    ACITermData mData;
};
ACITerm ACITerm_Construct(ACITerm_Type aType, Setting_Type aSetting);
void ACITerm_Destruct(ACITerm& aSelf);

std::string ACITerm_ToString(const ACITerm& aSelf);

struct ArbitraryCalculationInput
{
    std::vector<ACITerm> mTerms;
    Setting_Type mSetting;
    int mPower;
};
void ArbitraryCalculationInput_Destruct(ArbitraryCalculationInput& aSelf);

ArbitraryCalculationInput ArbitraryCalculationInput_FromString(ParsingInfo& aParsingInfo, Setting_Type aSetting);

std::string ArbitraryCalculationInput_ToString(const ArbitraryCalculationInput& aACI);

void ArbitraryCalculationInput_CoagulateInnermostToPoly_Recursive(ArbitraryCalculationInput& aACI);

bool ArbitraryCalculationInput_IsOnlyPolynomial(const ArbitraryCalculationInput& aACI);

bool ArbitraryCalculationInput_IsOnlyPower1Polynomial(const ArbitraryCalculationInput& aACI);

void ArbitraryCalculationInput_ExpandPolyExponent_Recursive(ArbitraryCalculationInput& aACI);

void ArbitraryCalculationInput_ExpandFoil_Recursive(ArbitraryCalculationInput& aACI);

void ArbitraryCalculationInput_UnsubPower1Poly_Recursive(ArbitraryCalculationInput& aACI);

void ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(void* aPolynomialOut, ArbitraryCalculationInput& aACI);
}

#endif // CORE_INCLUDE_ARBITRARY_CALCULATION_INPUT_HPP

