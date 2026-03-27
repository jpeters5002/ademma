#include "arbitrary_calculation_input.hpp"

#include "classical_adem_polynomial.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_polynomial.hpp"

#include "debug_output.hpp"

#include <string>
#include <assert.h>

// PRIVATE FUNCTION DECLARATIONS

namespace ademma_core
{
std::string ACITerm_ToString_Recursive(const ACITerm& aSelf);
void ArbitraryCalculationInput_FromString_Recursive(ArbitraryCalculationInput& aACIOut, ParsingInfo& aParsingInfo, Setting_Type aSetting, int aPower);
std::string ArbitraryCalculationInput_ToString_Recursive(const ArbitraryCalculationInput& aACI);
char prev_non_whitespace_char_in_string(const std::string& aStr, size_t aHighIndexExcluded, size_t* aCharAtIndex);
void ArbitraryCalculationInput_AddPolynomialAsMonomialTerms(ArbitraryCalculationInput& aACI, void* aPoly, Setting_Type aSetting);
}

ademma_core::ACITerm ademma_core::ACITerm_Construct(ACITerm_Type aType, Setting_Type aSetting)
{
    ACITerm aci_termOut {aType, aSetting, nullptr};
    switch (aci_termOut.mType)
    {
        case ACITerm_Type::cADD:
        case ACITerm_Type::cMULTIPLY:
            break;
        case ACITerm_Type::cSUBACI:
            aci_termOut.mData.mSubACI = new ArbitraryCalculationInput();
            break;
        case ACITerm_Type::cPOLYNOMIAL:
            switch (aci_termOut.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    aci_termOut.mData.mClassicalAdemPolynomial = new ClassicalAdemPolynomial();
                    break;
                case Setting_Type::cC_MOTIVIC:
                    aci_termOut.mData.mCMotivicAdemPolynomial = new CMotivicAdemPolynomial();
                    break;
                case Setting_Type::cR_MOTIVIC:
                    aci_termOut.mData.mRMotivicAdemPolynomial = new RMotivicAdemPolynomial();
                    break;
            }
            break;
        case ACITerm_Type::cMONOMIAL:
            switch (aci_termOut.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    aci_termOut.mData.mClassicalAdemMonomial = new ClassicalAdemMonomial();
                    break;
                case Setting_Type::cC_MOTIVIC:
                    aci_termOut.mData.mCMotivicAdemMonomial = new CMotivicAdemMonomial();
                    break;
                case Setting_Type::cR_MOTIVIC:
                    aci_termOut.mData.mRMotivicAdemMonomial = new RMotivicAdemMonomial();
                    break;
            }
            break;
        case ACITerm_Type::cNONE:
            break;
    }
    return aci_termOut;
}

void ademma_core::ACITerm_Destruct(ACITerm& aSelf)
{
    switch (aSelf.mType)
    {
        case ACITerm_Type::cADD:
        case ACITerm_Type::cMULTIPLY:
            break;
        case ACITerm_Type::cSUBACI:
            ArbitraryCalculationInput_Destruct(*aSelf.mData.mSubACI);
            delete aSelf.mData.mSubACI;
            break;
        case ACITerm_Type::cPOLYNOMIAL:
            switch (aSelf.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    delete aSelf.mData.mClassicalAdemPolynomial;
                    break;
                case Setting_Type::cC_MOTIVIC:
                    delete aSelf.mData.mCMotivicAdemPolynomial;
                    break;
                case Setting_Type::cR_MOTIVIC:
                    delete aSelf.mData.mRMotivicAdemPolynomial;
                    break;
            }
            break;
        case ACITerm_Type::cMONOMIAL:
            switch (aSelf.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    delete aSelf.mData.mClassicalAdemMonomial;
                    break;
                case Setting_Type::cC_MOTIVIC:
                    delete aSelf.mData.mCMotivicAdemMonomial;
                    break;
                case Setting_Type::cR_MOTIVIC:
                    delete aSelf.mData.mRMotivicAdemMonomial;
                    break;
            }
            break;
        case ACITerm_Type::cNONE:
            break;
    }
}

void ademma_core::ArbitraryCalculationInput_Destruct(ArbitraryCalculationInput& aSelf)
{
    for (size_t i = 0; i < aSelf.mTerms.size(); i++)
    {
        ACITerm_Destruct(aSelf.mTerms[i]);
    }
    aSelf.mTerms.clear();
}

std::string ademma_core::ACITerm_ToString(const ACITerm& aSelf)
{
    return ACITerm_ToString_Recursive(aSelf);
}

ademma_core::ArbitraryCalculationInput ademma_core::ArbitraryCalculationInput_FromString(ParsingInfo& aParsingInfo, Setting_Type aSetting)
{
    ArbitraryCalculationInput aciOut {};
    ArbitraryCalculationInput_FromString_Recursive(aciOut, aParsingInfo, aSetting, 1);
    return aciOut;
}

std::string ademma_core::ArbitraryCalculationInput_ToString(const ArbitraryCalculationInput& aACI)
{
    std::string strOut {};
    if (aACI.mPower != 1)
    {
        strOut += "(";
    }
    strOut += ArbitraryCalculationInput_ToString_Recursive(aACI);
    if (aACI.mPower != 1)
    {
        strOut += ")^{" + std::to_string(aACI.mPower) + "}";
    }
    return strOut;
}

#define COAGULATEINNERMOST_SETTINGIMPL(aci, aci_term_ptr, setting_ucc) \
    { \
        setting_ucc##AdemPolynomial poly##setting_ucc {}; \
        assert(aci.mTerms.size() % 2 == 1); \
        for (size_t i = 0; i < aci.mTerms.size(); i++) \
        { \
            if (i % 2 == 0) \
            { \
                aci_term_ptr = &aci.mTerms[i]; \
                switch (aci_term_ptr->mType) \
                { \
                    case ACITerm_Type::cMONOMIAL: \
                        poly##setting_ucc.push_back(*aci_term_ptr->mData.m##setting_ucc##AdemMonomial); \
                        break; \
                    case ACITerm_Type::cPOLYNOMIAL: \
                        setting_ucc##AdemPolynomial_AddRightPolynomial(poly##setting_ucc, *aci_term_ptr->mData.m##setting_ucc##AdemPolynomial); \
                        break; \
                    default: \
                        assert(!"unreachable"); \
                } \
            } \
            else \
            { \
                assert(aci.mTerms[i].mType == ACITerm_Type::cADD); \
            } \
        } \
        ArbitraryCalculationInput_Destruct(aci); \
        aci.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aci.mSetting)); \
        aci_term_ptr = &aci.mTerms[0]; \
        *aci_term_ptr->mData.m##setting_ucc##AdemPolynomial = poly##setting_ucc; \
    } do {} while (0)

void ademma_core::ArbitraryCalculationInput_CoagulateInnermostToPoly_Recursive(ArbitraryCalculationInput& aACI)
{
    bool contains_subaci = false;
    for (size_t i = 0; i < aACI.mTerms.size(); i++)
    {
        if (aACI.mTerms[i].mType == ACITerm_Type::cSUBACI)
        {
            contains_subaci = true;
            ArbitraryCalculationInput_CoagulateInnermostToPoly_Recursive(*aACI.mTerms[i].mData.mSubACI);
        }
    }
    ACITerm* aci_term_ptr;
    if (!contains_subaci)
    {
        switch (aACI.mSetting)
        {
            case Setting_Type::cCLASSICAL:
                COAGULATEINNERMOST_SETTINGIMPL(aACI, aci_term_ptr, Classical);
                break;
            case Setting_Type::cC_MOTIVIC:
                COAGULATEINNERMOST_SETTINGIMPL(aACI, aci_term_ptr, CMotivic);
                break;
            case Setting_Type::cR_MOTIVIC:
                COAGULATEINNERMOST_SETTINGIMPL(aACI, aci_term_ptr, RMotivic);
                break;
        }
    }
}

bool ademma_core::ArbitraryCalculationInput_IsOnlyPolynomial(const ArbitraryCalculationInput& aACI)
{
    return aACI.mTerms.size() == 1 && aACI.mTerms[0].mType == ACITerm_Type::cPOLYNOMIAL;
}

bool ademma_core::ArbitraryCalculationInput_IsOnlyPower1Polynomial(const ArbitraryCalculationInput& aACI)
{
    return ArbitraryCalculationInput_IsOnlyPolynomial(aACI) && aACI.mPower == 1;
}

void ademma_core::ArbitraryCalculationInput_ExpandPolyExponent_Recursive(ArbitraryCalculationInput& aACI)
{
    ACITerm* aci_term_ptr;
    ArbitraryCalculationInput* powered_subaci_ptr;
    ArbitraryCalculationInput* subaci_ptr;
    int power;
    for (size_t i = 0; i < aACI.mTerms.size(); i++)
    {
        aci_term_ptr = &aACI.mTerms[i];
        switch (aci_term_ptr->mType)
        {
            case ACITerm_Type::cADD:
            case ACITerm_Type::cMULTIPLY:
                assert(i > 0);
                assert(i < aACI.mTerms.size() - 1);
                break;
            case ACITerm_Type::cSUBACI:
                powered_subaci_ptr = aci_term_ptr->mData.mSubACI;
                assert(powered_subaci_ptr->mPower > 0);
                if (ArbitraryCalculationInput_IsOnlyPolynomial(*powered_subaci_ptr) && powered_subaci_ptr->mPower > 1)
                {
                    power = powered_subaci_ptr->mPower;
                    for (size_t j = 1; j < (size_t)power; j++)
                    {
                        i++;
                        aACI.mTerms.insert(aACI.mTerms.begin() + i, ACITerm_Construct(ACITerm_Type::cMULTIPLY, aACI.mSetting));
                        i++;
                        aACI.mTerms.insert(aACI.mTerms.begin() + i, ACITerm_Construct(ACITerm_Type::cSUBACI, aACI.mSetting));
                        subaci_ptr = aACI.mTerms[i].mData.mSubACI;
                        subaci_ptr->mSetting = aACI.mSetting;
                        subaci_ptr->mPower = 1;
                        subaci_ptr->mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aACI.mSetting));
                        switch (aACI.mSetting)
                        {
                            case Setting_Type::cCLASSICAL:
                                *subaci_ptr->mTerms[0].mData.mClassicalAdemPolynomial = *powered_subaci_ptr->mTerms[0].mData.mClassicalAdemPolynomial;
                                break;
                            case Setting_Type::cC_MOTIVIC:
                                *subaci_ptr->mTerms[0].mData.mCMotivicAdemPolynomial = *powered_subaci_ptr->mTerms[0].mData.mCMotivicAdemPolynomial;
                                break;
                            case Setting_Type::cR_MOTIVIC:
                                *subaci_ptr->mTerms[0].mData.mRMotivicAdemPolynomial = *powered_subaci_ptr->mTerms[0].mData.mRMotivicAdemPolynomial;
                                break;
                        }
                        powered_subaci_ptr->mPower--;
                    }
                }
                else
                {
                    ArbitraryCalculationInput_ExpandPolyExponent_Recursive(*powered_subaci_ptr);
                }
                break;
            case ACITerm_Type::cMONOMIAL:
            case ACITerm_Type::cPOLYNOMIAL:
                break;
            case ACITerm_Type::cNONE:
                assert(!"unreachable");
                break;
        }
    }
}

#define ENSUREPOWER1POLYNOMIALORMONOMIAL_BREAKIFNOT(aci_term_ptr) \
    if (aci_term_ptr->mType != ACITerm_Type::cPOLYNOMIAL) \
    { \
        if (aci_term_ptr->mType == ACITerm_Type::cMONOMIAL) \
        {} \
        else if (aci_term_ptr->mType == ACITerm_Type::cSUBACI && ArbitraryCalculationInput_IsOnlyPower1Polynomial(*aci_term_ptr->mData.mSubACI)) \
        { \
            aci_term_ptr = &aci_term_ptr->mData.mSubACI->mTerms[0]; \
        } \
        else \
        { \
            break; \
        } \
    } \
    assert(aci_term_ptr->mType == ACITerm_Type::cMONOMIAL && aci_term_ptr->mType == ACITerm_Type::cPOLYNOMIAL); \
    do {} while(0)

#define POLYNOMIALORMONOMIAL_MULTIPLYINTO_SETTINGIMPL(aci_left_ptr, aci_right_ptr, aci_product_ptr, setting_ucc) \
    if (aci_left_ptr->mType == ACITerm_Type::cPOLYNOMIAL && aci_right_ptr->mType == ACITerm_Type::cPOLYNOMIAL) \
    { \
        /* poly multiply */ \
        *aci_product_ptr->mData.m##setting_ucc##AdemPolynomial = setting_ucc##AdemPolynomial_MultiplyPolynomial(*aci_left_ptr->mData.m##setting_ucc##AdemPolynomial, *aci_right_ptr->mData.m##setting_ucc##AdemPolynomial); \
    } \
    else if (aci_left_ptr->mType == ACITerm_Type::cMONOMIAL && aci_right_ptr->mType == ACITerm_Type::cPOLYNOMIAL) \
    { \
        /* multiply left monomial then copy */ \
        setting_ucc##AdemPolynomial_MultiplyLeftMonomial(*aci_left_ptr->mData.m##setting_ucc##AdemMonomial, *aci_right_ptr->mData.m##setting_ucc##AdemPolynomial); \
        *aci_product_ptr->mData.m##setting_ucc##AdemPolynomial = *aci_right_ptr->mData.m##setting_ucc##AdemPolynomial; \
    } \
    else if (aci_left_ptr->mType == ACITerm_Type::cPOLYNOMIAL && aci_right_ptr->mType == ACITerm_Type::cMONOMIAL) \
    { \
        /* multiply right monomial then copy */ \
        setting_ucc##AdemPolynomial_MultiplyRightMonomial(*aci_left_ptr->mData.m##setting_ucc##AdemPolynomial, *aci_right_ptr->mData.m##setting_ucc##AdemMonomial); \
        *aci_product_ptr->mData.m##setting_ucc##AdemPolynomial = *aci_left_ptr->mData.m##setting_ucc##AdemPolynomial; \
    } \
    else \
    { \
        assert(aci_left_ptr->mType == ACITerm_Type::cMONOMIAL && aci_right_ptr->mType == ACITerm_Type::cMONOMIAL); \
        /* multiply monomial terms, form poly with one term, then copy all in one step */ \
        *aci_product_ptr->mData.m##setting_ucc##AdemPolynomial = { setting_ucc##AdemMonomial_Multiply(*aci_left_ptr->mData.m##setting_ucc##AdemMonomial, *aci_right_ptr->mData.m##setting_ucc##AdemMonomial) }; \
    } do {} while(0)

void ademma_core::ArbitraryCalculationInput_ExpandFoil_Recursive(ArbitraryCalculationInput& aACI)
{
    ACITerm* aci_term_left_poly_or_mono_factor_ptr;
    ACITerm* aci_term_right_poly_or_mono_factor_ptr;
    ACITerm* aci_term_product_ptr;
    ACITerm* aci_term_ptr;
    for (size_t i = 0; i < aACI.mTerms.size(); i++)
    {
        aci_term_ptr = &aACI.mTerms[i];
        switch (aci_term_ptr->mType)
        {
            case ACITerm_Type::cADD:
                assert(i > 0);
                assert(i < aACI.mTerms.size() - 1);
                break;
            case ACITerm_Type::cMULTIPLY:
                assert(i > 0);
                assert(i < aACI.mTerms.size() - 1);
                aci_term_left_poly_or_mono_factor_ptr = &aACI.mTerms[i - 1];
                ENSUREPOWER1POLYNOMIALORMONOMIAL_BREAKIFNOT(aci_term_left_poly_or_mono_factor_ptr);
                aci_term_right_poly_or_mono_factor_ptr = &aACI.mTerms[i + 1];
                ENSUREPOWER1POLYNOMIALORMONOMIAL_BREAKIFNOT(aci_term_right_poly_or_mono_factor_ptr);
                aACI.mTerms.insert(aACI.mTerms.begin() + (i + 2), ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aACI.mSetting));
                aci_term_product_ptr = &aACI.mTerms[i + 2];
                switch (aACI.mSetting)
                {
                    case Setting_Type::cCLASSICAL:
                        POLYNOMIALORMONOMIAL_MULTIPLYINTO_SETTINGIMPL(aci_term_left_poly_or_mono_factor_ptr, aci_term_right_poly_or_mono_factor_ptr, aci_term_product_ptr, Classical);
                        break;
                    case Setting_Type::cC_MOTIVIC:
                        POLYNOMIALORMONOMIAL_MULTIPLYINTO_SETTINGIMPL(aci_term_left_poly_or_mono_factor_ptr, aci_term_right_poly_or_mono_factor_ptr, aci_term_product_ptr, CMotivic);
                        break;
                    case Setting_Type::cR_MOTIVIC:
                        POLYNOMIALORMONOMIAL_MULTIPLYINTO_SETTINGIMPL(aci_term_left_poly_or_mono_factor_ptr, aci_term_right_poly_or_mono_factor_ptr, aci_term_product_ptr, RMotivic);
                        break;
                }
                ACITerm_Destruct(aACI.mTerms[i + 1]);
                ACITerm_Destruct(aACI.mTerms[i]);
                ACITerm_Destruct(aACI.mTerms[i - 1]);
                aACI.mTerms.erase(aACI.mTerms.begin() + (i + 1));
                aACI.mTerms.erase(aACI.mTerms.begin() + i);
                aACI.mTerms.erase(aACI.mTerms.begin() + (i - 1));
                i--;
                break;
            case ACITerm_Type::cSUBACI:
                if (!ArbitraryCalculationInput_IsOnlyPower1Polynomial(*aci_term_ptr->mData.mSubACI))
                {
                    ArbitraryCalculationInput_ExpandFoil_Recursive(*aci_term_ptr->mData.mSubACI);
                }
                break;
            case ACITerm_Type::cPOLYNOMIAL:
            case ACITerm_Type::cMONOMIAL:
                break;
            case ACITerm_Type::cNONE:
                assert(!"unreachable");
                break;
        }
    }
}

void ademma_core::ArbitraryCalculationInput_UnsubPower1Poly_Recursive(ArbitraryCalculationInput& aACI)
{
    ACITerm* aci_term_ptr;
    void* subaci_poly_term_data_ptr;
    for (size_t i = 0; i < aACI.mTerms.size(); i++)
    {
        aci_term_ptr = &aACI.mTerms[i];
        switch (aci_term_ptr->mType)
        {
            case ACITerm_Type::cADD:
            case ACITerm_Type::cMULTIPLY:
                break;
            case ACITerm_Type::cSUBACI:
                if (ArbitraryCalculationInput_IsOnlyPower1Polynomial(*aci_term_ptr->mData.mSubACI))
                {
                    // works regardless of setting due to just moving pointers around so use Classical
                    subaci_poly_term_data_ptr = aci_term_ptr->mData.mSubACI->mTerms[0].mData.mClassicalAdemPolynomial;
                    aci_term_ptr->mData.mSubACI->mTerms.clear();
                    ACITerm_Destruct(*aci_term_ptr);
                    aACI.mTerms[i].mType = ACITerm_Type::cPOLYNOMIAL;
                    aACI.mTerms[i].mData.mClassicalAdemPolynomial = reinterpret_cast<ClassicalAdemPolynomial*>(subaci_poly_term_data_ptr);
                }
                else
                {
                    ArbitraryCalculationInput_UnsubPower1Poly_Recursive(*aci_term_ptr->mData.mSubACI);
                }
                break;
            case ACITerm_Type::cPOLYNOMIAL:
            case ACITerm_Type::cMONOMIAL:
                break;
            case ACITerm_Type::cNONE:
                assert(!"unreachable");
                break;
        }
    }
}

void ademma_core::ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(void* aPolynomialOut, ArbitraryCalculationInput& aACI)
{
    DEBUG_PRINT("ACI_ExpandToPolynomial start: " + ArbitraryCalculationInput_ToString(aACI));
    for (;;)
    {
        ArbitraryCalculationInput_CoagulateInnermostToPoly_Recursive(aACI);
        DEBUG_PRINT("ACI_ExpandToPolynomial coagulated: " + ArbitraryCalculationInput_ToString(aACI));
        if (ArbitraryCalculationInput_IsOnlyPower1Polynomial(aACI))
        {
            switch (aACI.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    *reinterpret_cast<ClassicalAdemPolynomial*>(aPolynomialOut) = *aACI.mTerms[0].mData.mClassicalAdemPolynomial;
                    DEBUG_PRINT("ACI_ExpandToPolynomial done: " + ClassicalAdemPolynomial_ToString(*reinterpret_cast<ClassicalAdemPolynomial*>(aPolynomialOut)));
                    break;
                case Setting_Type::cC_MOTIVIC:
                    *reinterpret_cast<CMotivicAdemPolynomial*>(aPolynomialOut) = *aACI.mTerms[0].mData.mCMotivicAdemPolynomial;
                    DEBUG_PRINT("ACI_ExpandToPolynomial done: " + CMotivicAdemPolynomial_ToString(*reinterpret_cast<CMotivicAdemPolynomial*>(aPolynomialOut)));
                    break;
                case Setting_Type::cR_MOTIVIC:
                    *reinterpret_cast<RMotivicAdemPolynomial*>(aPolynomialOut) = *aACI.mTerms[0].mData.mRMotivicAdemPolynomial;
                    DEBUG_PRINT("ACI_ExpandToPolynomial done: " + RMotivicAdemPolynomial_ToString(*reinterpret_cast<RMotivicAdemPolynomial*>(aPolynomialOut)));
                    break;
            }
            break;
        }
        ArbitraryCalculationInput_ExpandPolyExponent_Recursive(aACI);
        DEBUG_PRINT("ACI_ExpandToPolynomial expanded poly exponent: " + ArbitraryCalculationInput_ToString(aACI));
        ArbitraryCalculationInput_ExpandFoil_Recursive(aACI);
        DEBUG_PRINT("ACI_ExpandToPolynomial expanded foil: " + ArbitraryCalculationInput_ToString(aACI));
        ArbitraryCalculationInput_UnsubPower1Poly_Recursive(aACI);
        DEBUG_PRINT("ACI_ExpandToPolynomial unsub power1 poly: " + ArbitraryCalculationInput_ToString(aACI));
    }
    ArbitraryCalculationInput_Destruct(aACI);
}

// PRIVATE FUNCTION DEFINITIONS

std::string ademma_core::ACITerm_ToString_Recursive(const ACITerm& aSelf)
{
    std::string strOut {};
    int power;
    switch (aSelf.mType)
    {
        case ACITerm_Type::cADD:
            strOut = " + ";
            break;
        case ACITerm_Type::cMULTIPLY:
            strOut = " * ";
            break;
        case ACITerm_Type::cSUBACI:
            power = aSelf.mData.mSubACI->mPower;
#if DEBUG_OUTPUT
            strOut += " SUB<";
#endif
            if (power != 1)
            {
                strOut += "(";
            }
            strOut += ArbitraryCalculationInput_ToString_Recursive(*aSelf.mData.mSubACI);
            if (power != 1)
            {
                strOut += ")";
                if (power != 1)
                {
                    strOut += "^{" + std::to_string(power) + "}";
                }
            }
#if DEBUG_OUTPUT
            strOut += ">SUB ";
#endif
            break;
        case ACITerm_Type::cPOLYNOMIAL:
#if DEBUG_OUTPUT
            strOut += " POLY<";
#endif
            switch (aSelf.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    strOut += ClassicalAdemPolynomial_ToString(*aSelf.mData.mClassicalAdemPolynomial);
                    break;
                case Setting_Type::cC_MOTIVIC:
                    strOut += CMotivicAdemPolynomial_ToString(*aSelf.mData.mCMotivicAdemPolynomial);
                    break;
                case Setting_Type::cR_MOTIVIC:
                    strOut += RMotivicAdemPolynomial_ToString(*aSelf.mData.mRMotivicAdemPolynomial);
                    break;
            }
#if DEBUG_OUTPUT
            strOut += ">POLY ";
#endif
            break;
        case ACITerm_Type::cMONOMIAL:
#if DEBUG_OUTPUT
            strOut += " MONO<";
#endif
            switch (aSelf.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    strOut += ClassicalAdemMonomial_ToString(*aSelf.mData.mClassicalAdemMonomial);
                    break;
                case Setting_Type::cC_MOTIVIC:
                    strOut += CMotivicAdemMonomial_ToString(*aSelf.mData.mCMotivicAdemMonomial);
                    break;
                case Setting_Type::cR_MOTIVIC:
                    strOut += RMotivicAdemMonomial_ToString(*aSelf.mData.mRMotivicAdemMonomial);
                    break;
            }
#if DEBUG_OUTPUT
            strOut += ">MONO ";
#endif
            break;
        case ACITerm_Type::cNONE:
            break;
    }
    return strOut;
}

#define POLYNOMIAL_FROMSTRING_INTOACIASMONOS_SETTINGIMPL(parse_info, setting, aci, cleanup_fail_exit_label, setting_ucc) \
    { \
        setting_ucc##AdemPolynomial poly##setting_ucc = setting_ucc##AdemPolynomial_FromString(parse_info); \
        if (parse_info.mErrorInfo.mIsError) \
        { \
            goto cleanup_fail_exit_label; \
        } \
        ArbitraryCalculationInput_AddPolynomialAsMonomialTerms(aci, &poly##setting_ucc, setting); \
    } \

#define POLYNOMIAL_FROMSTRING_INTOACI_ASMONOS(parse_info, setting, aci, cleanup_fail_exit_label) \
    switch (setting) \
    { \
        case Setting_Type::cCLASSICAL: \
            POLYNOMIAL_FROMSTRING_INTOACIASMONOS_SETTINGIMPL(parse_info, setting, aci, cleanup_fail_exit_label, Classical); \
            break; \
        case Setting_Type::cC_MOTIVIC: \
            POLYNOMIAL_FROMSTRING_INTOACIASMONOS_SETTINGIMPL(parse_info, setting, aci, cleanup_fail_exit_label, CMotivic); \
            break; \
        case Setting_Type::cR_MOTIVIC: \
            POLYNOMIAL_FROMSTRING_INTOACIASMONOS_SETTINGIMPL(parse_info, setting, aci, cleanup_fail_exit_label, RMotivic); \
            break; \
    } do {} while(0)

void ademma_core::ArbitraryCalculationInput_FromString_Recursive(ArbitraryCalculationInput& aACIOut, ParsingInfo& aParsingInfo, Setting_Type aSetting, int aPower)
{
    aACIOut.mSetting = aSetting;
    aACIOut.mPower = aPower;
    size_t l_paren_pos;
    ParsingInfo sub_parsing_info;
    std::string swapstr;
    for (;;)
    {
        l_paren_pos = aParsingInfo.mStringToParse.find("(", aParsingInfo.mCurrentIndex);
        size_t r_paren_pos;
        size_t sub_l_paren_pos;
        char prev_char;
        ACITerm_Type l_paren_binop_type;
        size_t l_cut_to_pos;
        bool nothing_on_left;
        size_t r_paren_after_power_pos;
        bool power_is_bracketed;
        int power;
        ACITerm* aci_term_ptr;
        if (l_paren_pos == std::string::npos)
        {
            POLYNOMIAL_FROMSTRING_INTOACI_ASMONOS(aParsingInfo, aSetting, aACIOut, cleanup_fail_exit);
            break;
        }
        r_paren_pos = aParsingInfo.mStringToParse.find(")", l_paren_pos + 1);
        if (r_paren_pos == std::string::npos)
        {
            goto error_no_r_paren_exit;
        }
        // ensure r_paren_pos is the position of the ')' that aligns with the found '('
        sub_l_paren_pos = l_paren_pos;
        for (;;)
        {
            sub_l_paren_pos = aParsingInfo.mStringToParse.find("(", sub_l_paren_pos + 1);
            if (sub_l_paren_pos == std::string::npos || sub_l_paren_pos > r_paren_pos)
            {
                break;
            }
            r_paren_pos = aParsingInfo.mStringToParse.find(")", r_paren_pos + 1);
            if (r_paren_pos == std::string::npos)
            {
                goto error_no_r_paren_exit;
            }
        }
        // l_paren_pos and r_paren_pos correct now and not npos
        prev_char = prev_non_whitespace_char_in_string(aParsingInfo.mStringToParse, l_paren_pos, &l_cut_to_pos);
        if (prev_char != '\0' && l_cut_to_pos < aParsingInfo.mCurrentIndex)
        {
            prev_char = '\0';
        }
        nothing_on_left = false;
        switch (prev_char)
        {
            case '+':
                // ... + (...) so cut up to '+', poly_fromstring, aciterm_add
                l_paren_binop_type = ACITerm_Type::cADD;
                break;
            case '*':
                // ... * (...) so cut up to '*', poly_fromstring, aciterm_multiply
                l_paren_binop_type = ACITerm_Type::cMULTIPLY;
                break;
            case '\0':
                // (...) so do nothing
                nothing_on_left = true;
                break;
            default:
                // ... (...) so cut up to '(', poly_fromstring, aciterm_multiply
                l_paren_binop_type = ACITerm_Type::cMULTIPLY;
                l_cut_to_pos = l_paren_pos;
                break;
        }
        if (!nothing_on_left)
        {
            sub_parsing_info = aParsingInfo;
            sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_cut_to_pos);
            POLYNOMIAL_FROMSTRING_INTOACI_ASMONOS(sub_parsing_info, aSetting, aACIOut, subparseinfo_error_exit);
            aACIOut.mTerms.push_back(ACITerm_Construct(l_paren_binop_type, aSetting));
        }
        aParsingInfo.mCurrentIndex = l_paren_pos + 1;
        // (...) - everything handled (left side)
        // now right side (power only)
        // (...) or (...) ^ {<num>} or (...) ^ <num> or bad input
        sub_parsing_info = aParsingInfo;
        sub_parsing_info.mCurrentIndex = r_paren_pos + 1;
        power = 1;
        if (sub_parsing_info.MatchString_IncreaseIndexOnSuccess("^"))
        {
            power_is_bracketed = false;
            if (sub_parsing_info.MatchString_IncreaseIndexOnSuccess("{"))
            {
                power_is_bracketed = true;
            }
            if (!sub_parsing_info.ParseInt(power))
            {
                aParsingInfo.mErrorInfo.mIsError = true;
                aParsingInfo.mErrorInfo.mErrorNearbyIndex = sub_parsing_info.mCurrentIndex;
                aParsingInfo.mErrorInfo.mErrorString = "Unable to parse int exponent of parentheses-contained input";
                goto cleanup_fail_exit;
            }
            if (power <= 0)
            {
                aParsingInfo.mErrorInfo.mIsError = true;
                aParsingInfo.mErrorInfo.mErrorNearbyIndex = sub_parsing_info.mCurrentIndex;
                aParsingInfo.mErrorInfo.mErrorString = "Power of parentheses-contained input section yeilded a non-positive int";
                goto cleanup_fail_exit;
            }
            sub_parsing_info.IncreaseIndexOverInt();
            if (power_is_bracketed)
            {
                if (!sub_parsing_info.MatchString_IncreaseIndexOnSuccess("}"))
                {
                    aParsingInfo.mErrorInfo.mIsError = true;
                    aParsingInfo.mErrorInfo.mErrorNearbyIndex = sub_parsing_info.mCurrentIndex;
                    aParsingInfo.mErrorInfo.mErrorString = "Left bracket '{' before exponent has no matching right bracket '}'.";
                    goto cleanup_fail_exit;
                }
            }
        }
        r_paren_after_power_pos = sub_parsing_info.mCurrentIndex;
        // optional exponent obtained from right of ')' into variable 'power'
        sub_parsing_info = aParsingInfo;
        sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, r_paren_pos);
        aACIOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cSUBACI, aSetting));
        aci_term_ptr = &aACIOut.mTerms[aACIOut.mTerms.size() - 1];
        assert(aci_term_ptr->mType == ACITerm_Type::cSUBACI);
        ArbitraryCalculationInput_FromString_Recursive(*aci_term_ptr->mData.mSubACI, sub_parsing_info, aSetting, power);
        if (sub_parsing_info.mErrorInfo.mIsError)
        {
            goto subparseinfo_error_exit;
        }
        aParsingInfo.mCurrentIndex = r_paren_after_power_pos;
        // left of, inside, and optional exponent after handled
        aParsingInfo.IncreaseIndexOverWhitespace();
        if (aParsingInfo.mCurrentIndex < aParsingInfo.mStringToParse.size())
        {
            switch (aParsingInfo.mStringToParse[aParsingInfo.mCurrentIndex])
            {
                case '+':
                    aParsingInfo.MatchString_IncreaseIndexOnSuccess("+");
                    aACIOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cADD, aSetting));
                    break;
                case '*':
                    aParsingInfo.MatchString_IncreaseIndexOnSuccess("*");
                    // falls through
                default:
                    aACIOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMULTIPLY, aSetting));
            }
        }
        else
        {
            break;
        }
    }
    return;
subparseinfo_error_exit:
    swapstr = aParsingInfo.mStringToParse;
    aParsingInfo = sub_parsing_info;
    aParsingInfo.mStringToParse = swapstr;
    goto cleanup_fail_exit;
error_no_r_paren_exit:
    aParsingInfo.mErrorInfo.mIsError = true;
    aParsingInfo.mErrorInfo.mErrorNearbyIndex = l_paren_pos;
    aParsingInfo.mErrorInfo.mErrorString = "No matching right parenthesis for given left parenthesis";
    goto cleanup_fail_exit;
cleanup_fail_exit:
    ArbitraryCalculationInput_Destruct(aACIOut);
    return;
}

std::string ademma_core::ArbitraryCalculationInput_ToString_Recursive(const ArbitraryCalculationInput& aACI)
{
    std::string strOut {};
    for (size_t i = 0; i < aACI.mTerms.size(); i++)
    {
        strOut += ACITerm_ToString_Recursive(aACI.mTerms[i]);
    }
    return strOut;
}

char ademma_core::prev_non_whitespace_char_in_string(const std::string& aStr, size_t aHighIndexExcluded, size_t* aCharAtIndex)
{
    if (aHighIndexExcluded == 0)
    {
        return '\0';
    }
    for (size_t i = aHighIndexExcluded - 1;; i--)
    {
        switch (aStr[i])
        {
            case ' ':
            case '\n':
            case '\t':
                break;
            default:
                if (aCharAtIndex)
                {
                    *aCharAtIndex = i;
                }
                return aStr[i];
        }
        if (i == 0)
        {
            break;
        }
    }
    return '\0';
}

#define ACI_ADDPOLYASMONOTERMS_SETTINGIMPL(aci, aci_term_ptr, setting_enum, setting_ucc) \
    for (size_t i = 0; i < poly##setting_ucc->size(); i++) \
    { \
        aACI.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMONOMIAL, aSetting)); \
        aci_term_ptr = &aACI.mTerms[aACI.mTerms.size() - 1]; \
        *aci_term_ptr->mData.m##setting_ucc##AdemMonomial = (*poly##setting_ucc)[i]; \
        if (i < poly##setting_ucc->size() - 1) \
        { \
            aACI.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cADD, aSetting)); \
        } \
    } do {} while(0)

void ademma_core::ArbitraryCalculationInput_AddPolynomialAsMonomialTerms(ArbitraryCalculationInput& aACI, void* aPoly, Setting_Type aSetting)
{
    ClassicalAdemPolynomial* polyClassical;
    CMotivicAdemPolynomial* polyCMotivic;
    RMotivicAdemPolynomial* polyRMotivic;
    ACITerm* aci_term_ptr;
    switch (aSetting)
    {
        case Setting_Type::cCLASSICAL:
            polyClassical = reinterpret_cast<ClassicalAdemPolynomial*>(aPoly);
            ACI_ADDPOLYASMONOTERMS_SETTINGIMPL(aACI, aci_term_ptr, aSetting, Classical);
            break;
        case Setting_Type::cC_MOTIVIC:
            polyCMotivic = reinterpret_cast<CMotivicAdemPolynomial*>(aPoly);
            ACI_ADDPOLYASMONOTERMS_SETTINGIMPL(aACI, aci_term_ptr, aSetting, CMotivic);
            break;
        case Setting_Type::cR_MOTIVIC:
            polyRMotivic = reinterpret_cast<RMotivicAdemPolynomial*>(aPoly);
            ACI_ADDPOLYASMONOTERMS_SETTINGIMPL(aACI, aci_term_ptr, aSetting, RMotivic);
            break;
    }
}

