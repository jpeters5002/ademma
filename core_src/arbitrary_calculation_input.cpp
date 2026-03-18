#include "arbitrary_calculation_input.hpp"

#include "classical_adem_polynomial.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_polynomial.hpp"

#include <string>
#include <assert.h>

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
    ArbitraryCalculationInput aciOut {{}, 1};
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
        if (l_paren_binop_mult_pos == std::string::npos && l_paren_binop_add_pos == std::string::npos)
        {
            // mult = npos, add = npos
            only_whitespace_in_section = true;
            for (size_t i = aParsingInfo.mCurrentIndex; i < l_paren_pos; i++)
            {
                if (aParsingInfo.mStringToParse[i] == ' ' || aParsingInfo.mStringToParse[i] == '\t' || aParsingInfo.mStringToParse[i] == '\n')
                {
                    continue;
                }
                only_whitespace_in_section = false;
                break;
            }
            if (!only_whitespace_in_section)
            {
                // monomial (or bad input) - handle as polynomial for ease of data structures
                sub_parsing_info = aParsingInfo;
                sub_parsing_info.mStringToParse = sub_parsing_info.mStringToParse.substr(0, l_paren_pos);
                aciOut.mTerms.push_back(ACITerm_Construct(ACITerm_Type::cPOLYNOMIAL, aSetting));
                aci_term_ptr = &aciOut.mTerms[aciOut.mTerms.size() - 1];
                POLYNOMIAL_FROMSTRING_INTOACITERM(sub_parsing_info, aSetting, aci_term_ptr, subparseinfo_error_exit);
            }
        }
        // TODO: else if ...
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

