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

void ademma_core::ClassicalAdemPolynomial_EliminateAllSq0Factors(ClassicalAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        ClassicalAdemMonomial_EliminateAllSq0Factors(aPolynomial[i]);
    }
}

void ademma_core::ClassicalAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors(ClassicalAdemPolynomial& aPolynomial)
{
    // strange reverse iteration with forward sub-iteration madness
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

