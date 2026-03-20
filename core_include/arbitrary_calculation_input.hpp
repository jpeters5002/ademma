#ifndef CORE_INCLUDE_ARBITRARY_CALCULATION_INPUT_HPP
#define CORE_INCLUDE_ARBITRARY_CALCULATION_INPUT_HPP

#include <vector>

#include "setting.hpp"
#include "parsing_info.hpp"

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
struct ACITerm
{
    const ACITerm_Type mType;
    const Setting_Type mSetting;
    void* mData;
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

void ArbitraryCalculationInput_CoagulateInnermostToPoly(ArbitraryCalculationInput& aACI);
}

#endif // CORE_INCLUDE_ARBITRARY_CALCULATION_INPUT_HPP

