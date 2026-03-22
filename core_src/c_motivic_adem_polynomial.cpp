#include "c_motivic_adem_polynomial.hpp"

#include "c_motivic_adem_monomial.hpp"
#include "debug_output.hpp"

#include <stdexcept>

std::string ademma_core::CMotivicAdemPolynomial_ToString(const CMotivicAdemPolynomial& aValue)
{
    std::string outStr = "";
    for (size_t i = 0; i < aValue.size(); i++)
    {
        if (i != 0)
        {
            if (outStr.size() > 0 && outStr[outStr.size() - 1] != ' ')
            {
                outStr += ' ';
            }
            outStr += "+ ";
        }
        outStr += CMotivicAdemMonomial_ToString(aValue[i]);
    }
    if (outStr.empty())
    {
        outStr = "0";
    }
    return outStr;
}

ademma_core::CMotivicAdemPolynomial ademma_core::CMotivicAdemPolynomial_FromString(ParsingInfo& aParsingInfo)
{
    CMotivicAdemPolynomial cmapOut {};
    bool looped_at_least_once = false;
    for (;;)
    {
        aParsingInfo.IncreaseIndexOverWhitespace();
        ParsingInfo monomial_subinfo = aParsingInfo;
        std::string::size_type plus_pos = monomial_subinfo.mStringToParse.find('+', monomial_subinfo.mCurrentIndex);
        if (std::string::npos != plus_pos)
        {
            monomial_subinfo.mStringToParse = monomial_subinfo.mStringToParse.substr(0, plus_pos);
        }
        CMotivicAdemMonomial cmam = CMotivicAdemMonomial_FromString(monomial_subinfo);
        if (monomial_subinfo.mErrorInfo.mIsError)
        {
            std::string full_parse_string = aParsingInfo.mStringToParse;
            aParsingInfo = monomial_subinfo;
            aParsingInfo.mStringToParse = full_parse_string;
            return {};
        }
        aParsingInfo.mCurrentIndex = monomial_subinfo.mCurrentIndex;
        if (cmam.size() == 0)
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "No monomial term";
            if (looped_at_least_once)
            {
                aParsingInfo.mErrorInfo.mErrorString += " after '+'";
            }
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return {};
        }
        cmapOut.push_back(cmam);
        if (aParsingInfo.mCurrentIndex >= aParsingInfo.mStringToParse.size())
        {
            break;
        }
        if (!aParsingInfo.MatchString_IncreaseIndexOnSuccess("+"))
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Expected '+' or end of data after parsing monomial, but found '" + std::to_string(aParsingInfo.mStringToParse[aParsingInfo.mCurrentIndex]) + "'";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return {};
        }
        looped_at_least_once = true;
    }
    return cmapOut;
}

void ademma_core::CMotivicAdemPolynomial_EliminateAllSq0Factors(CMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("CMotPoly_EliminateAllSq0Factors called on: " + CMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        CMotivicAdemMonomial_EliminateAllSq0Factors(aPolynomial[i]);
    }
    DEBUG_PRINT("CMotPoly_EliminateAllSq0Factors done:      " + CMotivicAdemPolynomial_ToString(aPolynomial));
}

void ademma_core::CMotivicAdemPolynomial_ShoveTauLeft(CMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("CMotPoly_ShoveTauLeft called on: " + CMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        CMotivicAdemMonomial_ShoveTauLeft(aPolynomial[i]);
    }
    DEBUG_PRINT("CMotPoly_ShoveTauLeft done:      " + CMotivicAdemPolynomial_ToString(aPolynomial));
}

void ademma_core::CMotivicAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors_AssumeTauLeft(CMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("CMotPoly_CombineLikeTerms_AssumeNoSq0Factors_AssumeTauLeft called on: " + CMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 1; i < aPolynomial.size();)
    {
        for (size_t j = 0; j < i;)
        {
            if (CMotivicAdemMonomial_IsEqualInForm(aPolynomial[i], aPolynomial[j]))
            {
                aPolynomial.erase(aPolynomial.begin() + i);
                aPolynomial.erase(aPolynomial.begin() + j);
                i--; // because j < i
                goto skip_i_increment;
            }
            else
            {
                j++;
            }
        }
        i++;
skip_i_increment:
        continue;
    }
    DEBUG_PRINT("CMotPoly_CombineLikeTerms_AssumeNoSq0Factors_AssumeTauLeft done:      " + CMotivicAdemPolynomial_ToString(aPolynomial));
}

bool ademma_core::CMotivicAdemPolynomial_IsEqualInForm(const CMotivicAdemPolynomial& aLeft, const CMotivicAdemPolynomial& aRight)
{
    if (aLeft.size() != aRight.size())
    {
        return false;
    }
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        if (!CMotivicAdemMonomial_IsEqualInForm(aLeft[i], aRight[i]))
        {
            return false;
        }
    }
    return true;
}

bool ademma_core::CMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(const CMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("CMotPoly_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors called on: " + CMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        if (!CMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            DEBUG_PRINT("CMotPoly_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors return: false");
            return false;
        }
    }
    DEBUG_PRINT("CMotPoly_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors return: true");
    return true;
}

ademma_core::CMotivicAdemPolynomial ademma_core::CMotivicAdemPolynomial_MultiplyPolynomial(const CMotivicAdemPolynomial& aLeft, const CMotivicAdemPolynomial& aRight)
{
    DEBUG_PRINT("CMotPoly_MultiplyPolynomial called with: (" + CMotivicAdemPolynomial_ToString(aLeft) + ")(" + CMotivicAdemPolynomial_ToString(aRight) + ")");
    CMotivicAdemPolynomial cmapOut {};
    for (size_t left_i = 0; left_i < aLeft.size(); left_i++)
    {
        for (size_t right_i = 0; right_i < aRight.size(); right_i++)
        {
            cmapOut.push_back(CMotivicAdemMonomial_Multiply(aLeft[left_i], aRight[right_i]));
        }
    }
    DEBUG_PRINT("CMotPoly_MultiplyPolynomial return: " + CMotivicAdemPolynomial_ToString(cmapOut));
    return cmapOut;
}

void ademma_core::CMotivicAdemPolynomial_AddRightPolynomial(CMotivicAdemPolynomial& aLeft, const CMotivicAdemPolynomial& aRight)
{
    for (size_t i = 0; i < aRight.size(); i++)
    {
        aLeft.push_back(aRight[i]);
    }
}

void ademma_core::CMotivicAdemPolynomial_MultiplyLeftMonomial(const CMotivicAdemMonomial& aLeft, CMotivicAdemPolynomial& aRight)
{
    DEBUG_PRINT("CMotPoly_MultiplyLeftMonomial called with: " + CMotivicAdemMonomial_ToString(aLeft) + "(" + CMotivicAdemPolynomial_ToString(aRight) + ")");
    for (size_t i = 0; i < aRight.size(); i++)
    {
        aRight[i] = CMotivicAdemMonomial_Multiply(aLeft, aRight[i]);
    }
    DEBUG_PRINT("CMotPoly_MultiplyLeftMonomial done:      " + CMotivicAdemPolynomial_ToString(aRight));
}

void ademma_core::CMotivicAdemPolynomial_MultiplyRightMonomial(CMotivicAdemPolynomial& aLeft, const CMotivicAdemMonomial& aRight)
{
    DEBUG_PRINT("CMotPoly_MultiplyRightMonomial called with: (" + CMotivicAdemPolynomial_ToString(aLeft) + ")" + CMotivicAdemMonomial_ToString(aRight));
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        aLeft[i] = CMotivicAdemMonomial_Multiply(aLeft[i], aRight);
    }
    DEBUG_PRINT("CMotPoly_MultiplyRightMonomial done:      " + CMotivicAdemPolynomial_ToString(aLeft));
}

void ademma_core::CMotivicAdemPolynomial_ReplaceTermWithPolynomial(CMotivicAdemPolynomial& aPolynomial, size_t aTermIndex, const CMotivicAdemPolynomial& aReplacementPolynomial)
{
    DEBUG_PRINT("CMotPoly_ReplaceTermWithPolynomial called with: " + CMotivicAdemPolynomial_ToString(aPolynomial) + ", " + std::to_string(aTermIndex) + ", " + CMotivicAdemPolynomial_ToString(aReplacementPolynomial));
    if (aTermIndex >= aPolynomial.size())
    {
        throw std::runtime_error("bad index");
    }
    aPolynomial.erase(aPolynomial.begin() + aTermIndex);
    aPolynomial.insert(aPolynomial.begin() + aTermIndex, aReplacementPolynomial.begin(), aReplacementPolynomial.end());
    DEBUG_PRINT("CMotPoly_ReplaceTermWithPolynomial done:      " + CMotivicAdemPolynomial_ToString(aPolynomial));
}

