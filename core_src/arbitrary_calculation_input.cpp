#include "arbitrary_calculation_input.hpp"

#include "classical_adem_polynomial.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_polynomial.hpp"

#include <string>
#include <assert.h>

// PRIVATE FUNCTION DECLARATIONS

namespace ademma_core
{
void ArbitraryCalculationInput_FromString_Recursive(ArbitraryCalculationInput& aACIOut, ParsingInfo& aParsingInfo, Setting_Type aSetting, int aPower);
bool only_whitespace_in_string_section(const std::string& aStr, size_t aLowIndex, size_t aHighIndexExcluded);
char prev_non_whitespace_char_in_string(const std::string& aStr, size_t aHighIndexExcluded, size_t* aCharAtIndex);
char next_non_whitespace_char_in_string(const std::string& aStr, size_t aLowIndex, size_t* aCharAtIndex);
void ArbitraryCalculationInput_AddPolynomialAsMonomialTerms(ArbitraryCalculationInput& aACI, void* aPoly, Setting_Type aSetting);
}

// PRIVATE FUNCTION DEFINITIONS

ademma_core::ACITerm ademma_core::ACITerm_Construct(ACITerm_Type aType, Setting_Type aSetting)
{
    ACITerm aci_termOut {aType, aSetting};
    switch (aci_termOut.mType)
    {
        case ACITerm_Type::cADD:
        case ACITerm_Type::cMULTIPLY:
            aci_termOut.mData = nullptr;
            break;
        case ACITerm_Type::cSUBACI:
            aci_termOut.mData = reinterpret_cast<void*>(new ArbitraryCalculationInput());
            break;
        case ACITerm_Type::cPOLYNOMIAL:
            switch (aci_termOut.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    aci_termOut.mData = reinterpret_cast<void*>(new ClassicalAdemPolynomial());
                    break;
                case Setting_Type::cC_MOTIVIC:
                    aci_termOut.mData = reinterpret_cast<void*>(new CMotivicAdemPolynomial());
                    break;
                case Setting_Type::cR_MOTIVIC:
                    aci_termOut.mData = reinterpret_cast<void*>(new RMotivicAdemPolynomial());
                    break;
            }
            break;
        case ACITerm_Type::cMONOMIAL:
            switch (aci_termOut.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    aci_termOut.mData = reinterpret_cast<void*>(new ClassicalAdemMonomial());
                    break;
                case Setting_Type::cC_MOTIVIC:
                    aci_termOut.mData = reinterpret_cast<void*>(new CMotivicAdemMonomial());
                    break;
                case Setting_Type::cR_MOTIVIC:
                    aci_termOut.mData = reinterpret_cast<void*>(new RMotivicAdemMonomial());
                    break;
            }
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
            ArbitraryCalculationInput_Destruct(*reinterpret_cast<ArbitraryCalculationInput*>(aSelf.mData));
            delete reinterpret_cast<ArbitraryCalculationInput*>(aSelf.mData);
            break;
        case ACITerm_Type::cPOLYNOMIAL:
            switch (aSelf.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    delete reinterpret_cast<ClassicalAdemPolynomial*>(aSelf.mData);
                    break;
                case Setting_Type::cC_MOTIVIC:
                    delete reinterpret_cast<CMotivicAdemPolynomial*>(aSelf.mData);
                    break;
                case Setting_Type::cR_MOTIVIC:
                    delete reinterpret_cast<RMotivicAdemPolynomial*>(aSelf.mData);
                    break;
            }
            break;
        case ACITerm_Type::cMONOMIAL:
            switch (aSelf.mSetting)
            {
                case Setting_Type::cCLASSICAL:
                    delete reinterpret_cast<ClassicalAdemMonomial*>(aSelf.mData);
                    break;
                case Setting_Type::cC_MOTIVIC:
                    delete reinterpret_cast<CMotivicAdemMonomial*>(aSelf.mData);
                    break;
                case Setting_Type::cR_MOTIVIC:
                    delete reinterpret_cast<RMotivicAdemMonomial*>(aSelf.mData);
                    break;
            }
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

ademma_core::ArbitraryCalculationInput ademma_core::ArbitraryCalculationInput_FromString(ParsingInfo& aParsingInfo, Setting_Type aSetting)
{
    ArbitraryCalculationInput aciOut {};
    ArbitraryCalculationInput_FromString_Recursive(aciOut, aParsingInfo, aSetting, 1);
    return aciOut;
}

// PRIVATE FUNCTION DEFINITIONS

void ademma_core::ArbitraryCalculationInput_FromString_Recursive(ArbitraryCalculationInput& aACIOut, ParsingInfo& aParsingInfo, Setting_Type aSetting, int aPower)
{
    aACIOut.mPower = aPower;
    size_t l_paren_pos;
    ParsingInfo sub_parsing_info;
    std::string swapstr;
    for (;;)
    {
        l_paren_pos = aParsingInfo.mStringToParse.find("(", aParsingInfo.mCurrentIndex);
        size_t r_paren_pos;
        size_t sub_l_paren_pos;
        size_t l_paren_binop_add_pos;
        size_t l_paren_binop_mult_pos;
        size_t l_paren_binop_pos;
        bool only_whitespace_in_section;
        char prev_char;
        ACITerm_Type l_paren_binop_type;
        size_t l_cut_to_pos;
        bool nothing_on_left;
        char next_char;
        size_t r_paren_after_power_pos;
        bool power_is_bracketed;
        int power;
        bool expected_success;
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
        if (l_cut_to_pos < aParsingInfo.mCurrentIndex)
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
                    aParsingInfo.mErrorInfo.mErrorNearbyIndex = r_paren_after_power_pos;
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
        ArbitraryCalculationInput_FromString_Recursive(*reinterpret_cast<ArbitraryCalculationInput*>(aci_term_ptr->mData), sub_parsing_info, aSetting, power);
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
                    expected_success = aParsingInfo.MatchString_IncreaseIndexOnSuccess("+");
                    assert(expected_success);
                    aACIOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cADD, aSetting));
                    break;
                case '*':
                    expected_success = aParsingInfo.MatchString_IncreaseIndexOnSuccess("*");
                    assert(expected_success);
                    // falls through
                default:
                    aACIOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMULTIPLY, aSetting));
            }
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

bool ademma_core::only_whitespace_in_string_section(const std::string& aStr, size_t aLowIndex, size_t aHighIndexExcluded)
{
    bool only_whitespace = true;
    for (size_t i = aLowIndex; i < aHighIndexExcluded && i < aStr.size(); i++)
    {
        switch (aStr[i])
        {
            case ' ':
            case '\n':
            case '\t':
                continue;
            default:
                only_whitespace = false;
                break;
        }
        break;
    }
    return only_whitespace;
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

char next_non_whitespace_char_in_string(const std::string& aStr, size_t aLowIndex, size_t* aCharAtIndex)
{
    for (size_t i = aLowIndex; i < aStr.size(); i++)
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
    }
    return '\0';
}

#define ACI_ADDPOLYASMONOTERMS_SETTINGIMPL(aci, aci_term_ptr, setting_enum, setting_ucc) \
    for (size_t i = 0; i < poly##setting_ucc->size(); i++) \
    { \
        aACI.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMONOMIAL, aSetting)); \
        aci_term_ptr = &aACI.mTerms[aACI.mTerms.size() - 1]; \
        mono##setting_ucc = new setting_ucc##AdemMonomial(); \
        *mono##setting_ucc = (*poly##setting_ucc)[i]; \
        aci_term_ptr->mData = reinterpret_cast<void*>(mono##setting_ucc); \
        if (i < poly##setting_ucc->size() - 1) \
        { \
            aACI.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cADD, aSetting)); \
        } \
    } do {} while(0)

void ademma_core::ArbitraryCalculationInput_AddPolynomialAsMonomialTerms(ArbitraryCalculationInput& aACI, void* aPoly, Setting_Type aSetting)
{
    ClassicalAdemPolynomial* polyClassical;
    ClassicalAdemMonomial* monoClassical;
    CMotivicAdemPolynomial* polyCMotivic;
    CMotivicAdemMonomial* monoCMotivic;
    RMotivicAdemPolynomial* polyRMotivic;
    RMotivicAdemMonomial* monoRMotivic;
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

