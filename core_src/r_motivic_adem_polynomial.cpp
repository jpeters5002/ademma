#include "r_motivic_adem_polynomial.hpp"

#include "r_motivic_adem_monomial.hpp"

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

void ademma_core::RMotivicAdemPolynomial_EliminateAllSq0Factors(RMotivicAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        RMotivicAdemMonomial_EliminateAllSq0Factors(aPolynomial[i]);
    }
}

void ademma_core::RMotivicAdemPolynomial_ShoveRhoLeft(RMotivicAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        RMotivicAdemMonomial_ShoveRhoLeft(aPolynomial[i]);
    }
}

void ademma_core::RMotivicAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors_AssumeRhoLeft(RMotivicAdemPolynomial& aPolynomial)
{
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
}

bool ademma_core::RMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(const RMotivicAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        if (!RMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            return false;
        }
    }
    return true;
}

ademma_core::RMotivicAdemPolynomial ademma_core::RMotivicAdemPolynomial_MultiplyPolynomial(const RMotivicAdemPolynomial& aLeft, const RMotivicAdemPolynomial& aRight)
{
    RMotivicAdemPolynomial capOut {};
    for (size_t left_i = 0; left_i < aLeft.size(); left_i++)
    {
        for (size_t right_i = 0; right_i < aRight.size(); right_i++)
        {
            capOut.push_back(RMotivicAdemMonomial_Multiply(aLeft[left_i], aRight[right_i]));
        }
    }
    return capOut;
}

void ademma_core::RMotivicAdemPolynomial_MultiplyLeftMonomial(const RMotivicAdemMonomial& aLeft, RMotivicAdemPolynomial& aRight)
{
    for (size_t i = 0; i < aRight.size(); i++)
    {
        aRight[i] = RMotivicAdemMonomial_Multiply(aLeft, aRight[i]);
    }
}

void ademma_core::RMotivicAdemPolynomial_MultiplyRightMonomial(RMotivicAdemPolynomial& aLeft, const RMotivicAdemMonomial& aRight)
{
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        aLeft[i] = RMotivicAdemMonomial_Multiply(aLeft[i], aRight);
    }
}

void ademma_core::RMotivicAdemPolynomial_ReplaceTermWithPolynomial(RMotivicAdemPolynomial& aPolynomial, size_t aTermIndex, const RMotivicAdemPolynomial& aReplacementPolynomial)
{
    if (aTermIndex >= aPolynomial.size())
    {
        throw std::runtime_error("bad index");
    }
    aPolynomial.erase(aPolynomial.begin() + aTermIndex);
    aPolynomial.insert(aPolynomial.begin() + aTermIndex, aReplacementPolynomial.begin(), aReplacementPolynomial.end());
}

