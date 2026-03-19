#include "arbitrary_calculation_input.hpp"

#include "classical_adem_polynomial.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_polynomial.hpp"

#include <string>
#include <assert.h>

// PRIVATE FUNCTION DECLARATIONS

namespace ademma_core
{
bool only_whitespace_in_string_section(const std::string& aStr, size_t aLowIndex, size_t aHighIndexExcluded);
ArbitraryCalculationInput ArbitraryCalculationInput_FromString_Recursive(ParsingInfo& aParsingInfo, Setting_Type aSetting, int aPower);
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
    }
}

void ademma_core::ArbitraryCalculationInput_Destruct(ArbitraryCalculationInput& aSelf)
{
    for (size_t i = 0; i < aSelf.mTerms.size(); i++)
    {
        ACITerm_Destruct(aSelf.mTerms[i]);
    }
}

#define POLYNOMIAL_FROMSTRING_INTOACITERM(parse_info, setting, aci_term_ptr, cleanup_fail_exit_label) \
    switch (setting) \
    { \
        case Setting_Type::cCLASSICAL: \
            *reinterpret_cast<ClassicalAdemPolynomial*>(aci_term_ptr->mData) = ClassicalAdemPolynomial_FromString(parse_info); \
            if (parse_info.mErrorInfo.mIsError) \
            { \
                goto cleanup_fail_exit_label; \
            } \
            break; \
        case Setting_Type::cC_MOTIVIC: \
            *reinterpret_cast<CMotivicAdemPolynomial*>(aci_term_ptr->mData) = CMotivicAdemPolynomial_FromString(parse_info); \
            if (parse_info.mErrorInfo.mIsError) \
            { \
                goto cleanup_fail_exit_label; \
            } \
            break; \
        case Setting_Type::cR_MOTIVIC: \
            *reinterpret_cast<RMotivicAdemPolynomial*>(aci_term_ptr->mData) = RMotivicAdemPolynomial_FromString(parse_info); \
            if (parse_info.mErrorInfo.mIsError) \
            { \
                goto cleanup_fail_exit_label; \
            } \
            break; \
    } do {} while(0)

ademma_core::ArbitraryCalculationInput ademma_core::ArbitraryCalculationInput_FromString(ParsingInfo& aParsingInfo, Setting_Type aSetting)
{
    return ArbitraryCalculationInput_FromString_Recursive(aParsingInfo, aSetting, 1);
}

// PRIVATE FUNCTION DEFINITIONS

ademma_core::ArbitraryCalculationInput ademma_core::ArbitraryCalculationInput_FromString_Recursive(ParsingInfo& aParsingInfo, Setting_Type aSetting, int aPower)
{
    ArbitraryCalculationInput aciOut {{}, aPower};
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
        ACITerm_Type l_paren_binop_type;
        bool only_whitespace_in_section;
        char next_char;
        size_t r_paren_after_power_pos;
        bool power_is_bracketed;
        int power;
        bool expected_success;
        ACITerm* aci_term_ptr;
        if (l_paren_pos == std::string::npos)
        {
            aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
            aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
            assert(aci_term->mData);
            POLYNOMIAL_FROMSTRING_INTOACITERM(aParsingInfo, aSetting, aci_term_ptr, cleanup_fail_exit);
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
        // l_paren_pos and r_paren_pos correct now
        l_paren_binop_mult_pos = aParsingInfo.mStringToParse.rfind("*", l_paren_pos);
        if (l_paren_binop_mult_pos != std::string::npos && l_paren_binop_mult_pos < aParsingInfo.mCurrentIndex)
        {
            l_paren_binop_mult_pos = std::string::npos;
        }
        l_paren_binop_add_pos = aParsingInfo.mStringToParse.rfind("+", l_paren_pos);
        if (l_paren_binop_add_pos != std::string::npos && l_paren_binop_add_pos < aParsingInfo.mCurrentIndex)
        {
            l_paren_binop_add_pos = std::string::npos;
        }
        // l_paren_binop_mult_pos and l_paren_binop_add_pos correct now
        if (l_paren_binop_mult_pos == std::string::npos)
        {
            if (l_paren_binop_add_pos == std::string::npos)
            {
                l_paren_binop_type = ACITerm_Type::cNONE;
                l_paren_binop_pos = std::string::npos;
            }
            else
            {
                l_paren_binop_type = ACITerm_Type::cADD;
                l_paren_binop_pos = l_paren_binop_add_pos;
            }
        }
        else
        {
            if (l_paren_binop_add_pos == std::string::npos)
            {
                l_paren_binop_type = ACITerm_Type::cMULTIPLY;
                l_paren_binop_pos = l_paren_binop_mult_pos;
            }
            else
            {
                l_paren_binop_type = (l_paren_binop_add_pos > l_paren_binop_mult_pos) ? ACITerm_Type::cADD : ACITerm_Type::cMULTIPLY;
                l_paren_binop_pos = (l_paren_binop_add_pos > l_paren_binop_mult_pos) ? l_paren_binop_add_pos : l_paren_binop_mult_pos;
            }
        }
        // l_paren_binop_pos and l_paren_binop_type correct now
        if (l_paren_binop_pos == std::string::npos)
        {
            // (...) or monomial (...) or bad input
            only_whitespace_in_section = only_whitespace_in_string_section(aParsingInfo.mStringToParse, aParsingInfo.mCurrentIndex, l_paren_pos);
            if (!only_whitespace_in_section)
            {
                // monomial (...) or bad input - handle as polynomial for ease of data structures
                sub_parsing_info = aParsingInfo;
                sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_pos);
                aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
                aParsingInfo.mCurrentIndex = sub_parsing_info.mCurrentIndex;
                aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMULTIPLY, aSetting));
            }
        }
        else
        {
            // mult != npos or add != npos
            // [polynomial +] monomial * (...) or polynomial + (...) or polynomial + monomial (...) or [polynomial +] monomial * monomial (...) or bad input
            only_whitespace_in_section = only_whitespace_in_string_section(aParsingInfo.mStringToParse, l_paren_binop_pos + 1, l_paren_pos);
            if (!only_whitespace_in_section)
            {
                // polynomial + monomial (...) or [polynomial +] monomial * monomial (...) or bad input
                if (l_paren_binop_type == ACITerm_Type::cADD)
                {
                    // polynomial + monomial (...) or bad input
                    sub_parsing_info = aParsingInfo;
                    sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_binop_add_pos);
                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                    aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                    POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
                    aParsingInfo.mCurrentIndex = sub_parsing_info.mCurrentIndex;
                    if (!aParsingInfo.MatchString_IncreaseIndexOnSuccess("+"))
                    {
                        aParsingInfo.mErrorInfo.mIsError = true;
                        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
                        aParsingInfo.mErrorInfo.mErrorString = "[INTERNAL ERROR] Perceived '+' character to be next in previous logic but didn't find it in this logic";
                        goto cleanup_fail_exit;
                    }
                    aParsingInfo.IncreaseIndexOverWhitespace();

                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cADD, aSetting));

                    sub_parsing_info = aParsingInfo;
                    sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_pos);
                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                    aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                    POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
                    aParsingInfo.mCurrentIndex = sub_parsing_info.mCurrentIndex;

                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMULTIPLY, aSetting));
                }
                else
                {
                    assert(l_paren_binop_type == ACITerm_Type::cMULTIPLY);
                    // [polynomial +] monomial * monomial (...) or bad input
                    if (l_paren_binop_add_pos == std::string::npos)
                    {
                        // monomial * monomial (...) or bad input
                        // handled after (the else case aligns after the 'polynomial +')
                    }
                    else
                    {
                        // polynomial + monomial * monomial (...) or bad input
                        sub_parsing_info = aParsingInfo;
                        sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_binop_add_pos);
                        aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                        aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                        POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
                        aParsingInfo.mCurrentIndex = sub_parsing_info.mCurrentIndex;
                        if (!aParsingInfo.MatchString_IncreaseIndexOnSuccess("+"))
                        {
                            aParsingInfo.mErrorInfo.mIsError = true;
                            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
                            aParsingInfo.mErrorInfo.mErrorString = "[INTERNAL ERROR] Perceived '+' character to be next in previous logic but didn't find it in this logic";
                            goto cleanup_fail_exit;
                        }
                        aParsingInfo.IncreaseIndexOverWhitespace();

                        aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cADD, aSetting));
                    }
                    // monomial * monomial (...) or bad input - possibly already handled a 'polynomial +'
                    sub_parsing_info = aParsingInfo;
                    sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_pos);
                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                    aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                    POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
                    aParsingInfo.mCurrentIndex = sub_parsing_info.mCurrentIndex;

                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMULTIPLY, aSetting));
                }
                // (...) - everything else handled
            }
            else
            {
                // [polynomial +] monomial * (...) or polynomial + (...) or bad input
                if (l_paren_binop_add_pos != std::string::npos)
                {
                    // polynomial + ...
                    sub_parsing_info = aParsingInfo;
                    sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_binop_add_pos);
                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                    aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                    POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
                    aParsingInfo.mCurrentIndex = sub_parsing_info.mCurrentIndex;
                    if (!aParsingInfo.MatchString_IncreaseIndexOnSuccess("+"))
                    {
                        aParsingInfo.mErrorInfo.mIsError = true;
                        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
                        aParsingInfo.mErrorInfo.mErrorString = "[INTERNAL ERROR] Perceived '+' character to be next in previous logic but didn't find it in this logic";
                        goto cleanup_fail_exit;
                    }
                    aParsingInfo.IncreaseIndexOverWhitespace();

                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cADD, aSetting));
                }
                // monomial * (...) or (...) or bad input
                only_whitespace_in_section = only_whitespace_in_string_section(aParsingInfo.mStringToParse, aParsingInfo.mCurrentIndex, l_paren_pos);
                if (!only_whitespace_in_section)
                {
                    // monomial * (...)
                    sub_parsing_info = aParsingInfo;
                    sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_pos);
                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                    aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                    POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
                    aParsingInfo.mCurrentIndex = sub_parsing_info.mCurrentIndex;
                    if (!aParsingInfo.MatchString_IncreaseIndexOnSuccess("*"))
                    {
                        aParsingInfo.mErrorInfo.mIsError = true;
                        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
                        aParsingInfo.mErrorInfo.mErrorString = "[INTERNAL ERROR] Perceived '*' character to be next in previous logic but didn't find it in this logic";
                        goto cleanup_fail_exit;
                    }
                    aParsingInfo.IncreaseIndexOverWhitespace();

                    aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cMULTIPLY, aSetting));
                }
            }
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
            // TODO: use the now-valid 'power' variable and l_paren_pos, r_paren_pos variables to recurse this function on what's inside the '(' ')' section, checking sub_parsing_info.mErrorInfo.mIsError for an error internal to that input section
            // TODO: use r_paren_after_power_pos to set into aParsingInfo.mCurrentIndex and check for a binary operator (a plus even not immediately after is bad news as that means we may have a polynomial on the right with no gosh dang stupid I should have written this function differently)
            // first check that you can't just split out your polynomials into monomials with some separate function that can be leveraged by the macro. If everything is in terms of monomials after FromString, then order of operations doesn't become a complete headache to manage.
        }
    }
    return aciOut;
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
    ArbitraryCalculationInput_Destruct(aciOut);
    return {};
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

