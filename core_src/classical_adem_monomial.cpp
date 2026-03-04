#include "classical_adem_monomial.hpp"

std::string ademma_core::ClassicalAdemMonomial_ToString(const ClassicalAdemMonomial& aValue)
{
    std::string outStr = "";
    for (size_t i = 0; i < aValue.size(); i++)
    {
        outStr += SteenrodSquareDegree_ToString(aValue[i]);
    }
    return outStr;
}

ademma_core::ClassicalAdemMonomial ademma_core::ClassicalAdemMonomial_FromString(ParsingInfo& aParsingInfo)
{
    ClassicalAdemMonomial camOut {};
    for (;;)
    {
        aParsingInfo.IncreaseIndexOverWhitespace();
        if (aParsingInfo.mCurrentIndex >= aParsingInfo.mStringToParse.size())
        {
            break;
        }
        SteenrodSquareDegree ssd = SteenrodSquareDegree_FromString(aParsingInfo);
        if (aParsingInfo.mErrorInfo.mIsError)
        {
            return {};
        }
        camOut.push_back(ssd);
    }
    return camOut;
}

void ademma_core::ClassicalAdemMonomial_EliminateAllSq0Factors(ClassicalAdemMonomial& aMonomial)
{
    for (size_t i = aMonomial.size(); i > 0; i--)
    {
        if (aMonomial[i - 1] == 0)
        {
            aMonomial.erase(aMonomial.begin() + (i - 1));
        }
    }
}

bool ademma_core::ClassicalAdemMonomial_IsEqualInForm(const ClassicalAdemMonomial& aLeft, const ClassicalAdemMonomial& aRight)
{
    if (aLeft.size() != aRight.size())
    {
        return false;
    }
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        if (aLeft[i] != aRight[i])
        {
            return false;
        }
    }
    return true;
}

bool ademma_core::ClassicalAdemMonomial_IsAdmissible_AssumeNoSq0Factors(const ClassicalAdemMonomial& aMonomial)
{
    if (aMonomial.size() < 2)
    {
        return true;
    }
    SteenrodSquareDegree previous_degree = aMonomial[0];
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        if (!SteenrodSquareDegree_IsPairAdmissible(previous_degree, aMonomial[i]))
        {
            return false;
        }
        previous_degree = aMonomial[i];
    }
    return true;
}

ademma_core::ClassicalAdemMonomial ademma_core::ClassicalAdemMonomial_Multiply(const ClassicalAdemMonomial& aLeft, const ClassicalAdemMonomial& aRight)
{
    ClassicalAdemMonomial camOut = aLeft;
    camOut.insert(std::end(camOut), std::begin(aRight), std::end(aRight));
    return camOut;
}

