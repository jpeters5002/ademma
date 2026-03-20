#include "classical_adem_polynomial.hpp"

#include "classical_adem_monomial.hpp"

#include <stdexcept>

std::string ademma_core::ClassicalAdemPolynomial_ToString(const ClassicalAdemPolynomial& aValue)
{
    std::string outStr = "";
    for (size_t i = 0; i < aValue.size(); i++)
    {
        if (i != 0)
        {
            outStr += " + ";
        }
        outStr += ClassicalAdemMonomial_ToString(aValue[i]);
    }
    if (outStr.empty())
    {
        outStr = "0";
    }
    return outStr;
}

ademma_core::ClassicalAdemPolynomial ademma_core::ClassicalAdemPolynomial_FromString(ParsingInfo& aParsingInfo)
{
    ClassicalAdemPolynomial capOut {};
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
        ClassicalAdemMonomial cam = ClassicalAdemMonomial_FromString(monomial_subinfo);
        if (monomial_subinfo.mErrorInfo.mIsError)
        {
            std::string full_parse_string = aParsingInfo.mStringToParse;
            aParsingInfo = monomial_subinfo;
            aParsingInfo.mStringToParse = full_parse_string;
            return {};
        }
        aParsingInfo.mCurrentIndex = monomial_subinfo.mCurrentIndex;
        if (cam.size() == 0)
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
        capOut.push_back(cam);
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
    return capOut;
}

void ademma_core::ClassicalAdemPolynomial_EliminateAllSq0Factors(ClassicalAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        ClassicalAdemMonomial_EliminateAllSq0Factors(aPolynomial[i]);
    }
}

void ademma_core::ClassicalAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors(ClassicalAdemPolynomial& aPolynomial)
{
    for (size_t i = 1; i < aPolynomial.size();)
    {
        for (size_t j = 0; j < i;)
        {
            if (ClassicalAdemMonomial_IsEqualInForm(aPolynomial[i], aPolynomial[j]))
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
}

bool ademma_core::ClassicalAdemPolynomial_IsEqualInForm(const ClassicalAdemPolynomial& aLeft, const ClassicalAdemPolynomial& aRight)
{
    if (aLeft.size() != aRight.size())
    {
        return false;
    }
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        if (!ClassicalAdemMonomial_IsEqualInForm(aLeft[i], aRight[i]))
        {
            return false;
        }
    }
    return true;
}

bool ademma_core::ClassicalAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(const ClassicalAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        if (!ClassicalAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            return false;
        }
    }
    return true;
}

ademma_core::ClassicalAdemPolynomial ademma_core::ClassicalAdemPolynomial_MultiplyPolynomial(const ClassicalAdemPolynomial& aLeft, const ClassicalAdemPolynomial& aRight)
{
    ClassicalAdemPolynomial capOut {};
    for (size_t left_i = 0; left_i < aLeft.size(); left_i++)
    {
        for (size_t right_i = 0; right_i < aRight.size(); right_i++)
        {
            capOut.push_back(ClassicalAdemMonomial_Multiply(aLeft[left_i], aRight[right_i]));
        }
    }
    return capOut;
}

void ademma_core::ClassicalAdemPolynomial_AddRightPolynomial(ClassicalAdemPolynomial& aLeft, const ClassicalAdemPolynomial& aRight)
{
    for (size_t i = 0; i < aRight.size(); i++)
    {
        aLeft.push_back(aRight[i]);
    }
}


void ademma_core::ClassicalAdemPolynomial_MultiplyLeftMonomial(const ClassicalAdemMonomial& aLeft, ClassicalAdemPolynomial& aRight)
{
    for (size_t i = 0; i < aRight.size(); i++)
    {
        aRight[i] = ClassicalAdemMonomial_Multiply(aLeft, aRight[i]);
    }
}

void ademma_core::ClassicalAdemPolynomial_MultiplyRightMonomial(ClassicalAdemPolynomial& aLeft, const ClassicalAdemMonomial& aRight)
{
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        aLeft[i] = ClassicalAdemMonomial_Multiply(aLeft[i], aRight);
    }
}

void ademma_core::ClassicalAdemPolynomial_ReplaceTermWithPolynomial(ClassicalAdemPolynomial& aPolynomial, size_t aTermIndex, const ClassicalAdemPolynomial& aReplacementPolynomial)
{
    if (aTermIndex >= aPolynomial.size())
    {
        throw std::runtime_error("bad index");
    }
    aPolynomial.erase(aPolynomial.begin() + aTermIndex);
    aPolynomial.insert(aPolynomial.begin() + aTermIndex, aReplacementPolynomial.begin(), aReplacementPolynomial.end());
}

