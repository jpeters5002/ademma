#include "r_motivic_adem_polynomial.hpp"

#include "r_motivic_adem_monomial.hpp"
#include "debug_output.hpp"

#include <stdexcept>

std::string ademma_core::RMotivicAdemPolynomial_ToString(const RMotivicAdemPolynomial& aValue)
{
    std::string outStr = "";
    for (size_t i = 0; i < aValue.size(); i++)
    {
        if (i != 0)
        {
            outStr += " + ";
        }
        outStr += RMotivicAdemMonomial_ToString(aValue[i]);
    }
    if (outStr.empty())
    {
        outStr = "0";
    }
    return outStr;
}

ademma_core::RMotivicAdemPolynomial ademma_core::RMotivicAdemPolynomial_FromString(ParsingInfo& aParsingInfo)
{
    RMotivicAdemPolynomial rmapOut {};
    bool looped_at_least_once = false;
    for (;;)
    {
        ParsingInfo monomial_subinfo = aParsingInfo;
        std::string::size_type plus_pos = monomial_subinfo.mStringToParse.find('+', monomial_subinfo.mCurrentIndex);
        if (std::string::npos != plus_pos)
        {
            monomial_subinfo.mStringToParse = monomial_subinfo.mStringToParse.substr(0, plus_pos);
        }
        RMotivicAdemMonomial rmam = RMotivicAdemMonomial_FromString(monomial_subinfo);
        if (monomial_subinfo.mErrorInfo.mIsError)
        {
            std::string full_parse_string = aParsingInfo.mStringToParse;
            aParsingInfo = monomial_subinfo;
            aParsingInfo.mStringToParse = full_parse_string;
            return {};
        }
        aParsingInfo.mCurrentIndex = monomial_subinfo.mCurrentIndex;
        if (rmam.size() == 0)
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
        rmapOut.push_back(rmam);
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
    return rmapOut;
}

void ademma_core::RMotivicAdemPolynomial_EliminateAllSq0Factors(RMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("RMotPoly_EliminateAllSq0Factors called on: " + RMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        RMotivicAdemMonomial_EliminateAllSq0Factors(aPolynomial[i]);
    }
    DEBUG_PRINT("RMotPoly_EliminateAllSq0Factors done:      " + RMotivicAdemPolynomial_ToString(aPolynomial));
}

void ademma_core::RMotivicAdemPolynomial_ShoveRhoLeft(RMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("RMotPoly_ShoveRhoLeft called on: " + RMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        RMotivicAdemMonomial_ShoveRhoLeft(aPolynomial[i]);
    }
    DEBUG_PRINT("RMotPoly_ShoveRhoLeft done:      " + RMotivicAdemPolynomial_ToString(aPolynomial));
}

void ademma_core::RMotivicAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors_AssumeRhoLeft(RMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("RMotPoly_CombineLikeTerms_AssumeNoSq0Factors_AssumeRhoLeft called on: " + RMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 1; i < aPolynomial.size();)
    {
        for (size_t j = 0; j < i;)
        {
            if (RMotivicAdemMonomial_IsEqualInForm(aPolynomial[i], aPolynomial[j]))
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
    DEBUG_PRINT("RMotPoly_CombineLikeTerms_AssumeNoSq0Factors_AssumeRhoLeft done:      " + RMotivicAdemPolynomial_ToString(aPolynomial));
}

bool ademma_core::RMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(const RMotivicAdemPolynomial& aPolynomial)
{
    DEBUG_PRINT("RMotPoly_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors called on: " + RMotivicAdemPolynomial_ToString(aPolynomial));
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        if (!RMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            DEBUG_PRINT("RMotPoly_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors return: false");
            return false;
        }
    }
    DEBUG_PRINT("RMotPoly_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors return: true");
    return true;
}

ademma_core::RMotivicAdemPolynomial ademma_core::RMotivicAdemPolynomial_MultiplyPolynomial(const RMotivicAdemPolynomial& aLeft, const RMotivicAdemPolynomial& aRight)
{
    DEBUG_PRINT("RMotPoly_MultiplyPolynomial called with: (" + RMotivicAdemPolynomial_ToString(aLeft) + ")(" + RMotivicAdemPolynomial_ToString(aRight) + ")");
    RMotivicAdemPolynomial rmapOut {};
    for (size_t left_i = 0; left_i < aLeft.size(); left_i++)
    {
        for (size_t right_i = 0; right_i < aRight.size(); right_i++)
        {
            rmapOut.push_back(RMotivicAdemMonomial_Multiply(aLeft[left_i], aRight[right_i]));
        }
    }
    DEBUG_PRINT("RMotPoly_MultiplyPolynomial return: " + RMotivicAdemPolynomial_ToString(rmapOut));
    return rmapOut;
}

void ademma_core::RMotivicAdemPolynomial_MultiplyLeftMonomial(const RMotivicAdemMonomial& aLeft, RMotivicAdemPolynomial& aRight)
{
    DEBUG_PRINT("RMotPoly_MultiplyLeftMonomial called with: " + RMotivicAdemMonomial_ToString(aLeft) + "(" + RMotivicAdemPolynomial_ToString(aRight) + ")");
    for (size_t i = 0; i < aRight.size(); i++)
    {
        aRight[i] = RMotivicAdemMonomial_Multiply(aLeft, aRight[i]);
    }
    DEBUG_PRINT("RMotPoly_MultiplyLeftMonomial done:      " + RMotivicAdemPolynomial_ToString(aRight));
}

void ademma_core::RMotivicAdemPolynomial_MultiplyRightMonomial(RMotivicAdemPolynomial& aLeft, const RMotivicAdemMonomial& aRight)
{
    DEBUG_PRINT("RMotPoly_MultiplyRightMonomial called with: (" + RMotivicAdemPolynomial_ToString(aLeft) + ")" + RMotivicAdemMonomial_ToString(aRight));
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        aLeft[i] = RMotivicAdemMonomial_Multiply(aLeft[i], aRight);
    }
    DEBUG_PRINT("RMotPoly_MultiplyRightMonomial done:      " + RMotivicAdemPolynomial_ToString(aLeft));
}

void ademma_core::RMotivicAdemPolynomial_ReplaceTermWithPolynomial(RMotivicAdemPolynomial& aPolynomial, size_t aTermIndex, const RMotivicAdemPolynomial& aReplacementPolynomial)
{
    DEBUG_PRINT("RMotPoly_ReplaceTermWithPolynomial called with: " + RMotivicAdemPolynomial_ToString(aPolynomial) + ", " + std::to_string(aTermIndex) + ", " + RMotivicAdemPolynomial_ToString(aReplacementPolynomial));
    if (aTermIndex >= aPolynomial.size())
    {
        throw std::runtime_error("bad index");
    }
    aPolynomial.erase(aPolynomial.begin() + aTermIndex);
    aPolynomial.insert(aPolynomial.begin() + aTermIndex, aReplacementPolynomial.begin(), aReplacementPolynomial.end());
    DEBUG_PRINT("RMotPoly_ReplaceTermWithPolynomial done:      " + RMotivicAdemPolynomial_ToString(aPolynomial));
}

