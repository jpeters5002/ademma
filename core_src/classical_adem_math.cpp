#include "classical_adem_math.hpp"

#include "classical_adem_polynomial.hpp"
#include "general_adem_math.hpp"

#include <stdexcept>

// PRIVATE FUNCTIONS DECLARATION

typedef unsigned int ademma_uint_t;

namespace ademma_core { namespace classical_adem_math
{
ClassicalAdemPolynomial admissify_two_factor_classical_adem_monomial(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight);
ClassicalAdemPolynomial admissify_classical_adem_monomial_one_step_AssumeNoSq0Factors(const ClassicalAdemMonomial& aMonomial);
void admissify_classical_adem_polynomial_one_step_AssumeNoSq0Factors(ClassicalAdemPolynomial& aPolynomial);
}}

// PUBLIC FUNCTIONS

ademma_core::ClassicalAdemPolynomial ademma_core::classical_adem_math::admissify_classical_adem_monomial(const ClassicalAdemMonomial& aMonomial)
{
    ClassicalAdemPolynomial capOut {};
    capOut.push_back(aMonomial);
    for (;;)
    {
        ClassicalAdemPolynomial_EliminateAllSq0Factors(capOut);
        ClassicalAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors(capOut);
        if (ClassicalAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(capOut))
        {
            break;
        }
        admissify_classical_adem_polynomial_one_step_AssumeNoSq0Factors(capOut);
    }
    return capOut;
}

// PRIVATE FUNCTIONS DEFINITION

ademma_core::ClassicalAdemPolynomial ademma_core::classical_adem_math::admissify_two_factor_classical_adem_monomial(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight)
{
    ClassicalAdemPolynomial cap {};
    if (SteenrodSquareDegree_IsPairAdmissible(aLeft, aRight))
    {
        ClassicalAdemMonomial cam {};
        cam.push_back(aLeft);
        cam.push_back(aRight);
        cap.push_back(cam);
    }
    else
    {
        for (int k = 0; k <= aLeft / 2; k++)
        {
            ademma_uint_t chooseTop = aRight - k - 1;
            ademma_uint_t chooseBottom = aLeft - 2 * k;
            if (chooseTop >= chooseBottom && ademma_core::general_adem_math::f2_choose(chooseTop, chooseBottom))
            {
                ClassicalAdemMonomial cam {};
                cam.push_back(aLeft + aRight - k);
                cam.push_back(k);
                cap.push_back(cam);
            }
        }
    }
    return cap;
}

ademma_core::ClassicalAdemPolynomial ademma_core::classical_adem_math::admissify_classical_adem_monomial_one_step_AssumeNoSq0Factors(const ClassicalAdemMonomial& aMonomial)
{
    ClassicalAdemPolynomial capOut {};
    if (ClassicalAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aMonomial))
    {
        capOut.push_back(aMonomial);
        return capOut;
    }
    ClassicalAdemMonomial left_leftover_monomial {};
    ClassicalAdemMonomial right_leftover_monomial {};
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        SteenrodSquareDegree left_degree = aMonomial[i - 1];
        SteenrodSquareDegree right_degree = aMonomial[i];
        if (SteenrodSquareDegree_IsPairAdmissible(left_degree, right_degree))
        {
            continue;
        }
        capOut = admissify_two_factor_classical_adem_monomial(aMonomial[i - 1], aMonomial [i]);
        for (size_t left_j = 0; left_j < i - 1; left_j++)
        {
            left_leftover_monomial.push_back(aMonomial[left_j]);
        }
        for (size_t right_j = i + 1; right_j < aMonomial.size(); right_j++)
        {
            right_leftover_monomial.push_back(aMonomial[right_j]);
        }
        ClassicalAdemPolynomial_MultiplyLeftMonomial(left_leftover_monomial, capOut);
        ClassicalAdemPolynomial_MultiplyRightMonomial(capOut, right_leftover_monomial);
        return capOut;
    }
    throw std::runtime_error("no inadmissible pair found within monomial that was checked to have an inadmissible pair somewhere");
}

void ademma_core::classical_adem_math::admissify_classical_adem_polynomial_one_step_AssumeNoSq0Factors(ClassicalAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size();)
    {
        if (ClassicalAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            i++;
            continue;
        }
        ClassicalAdemPolynomial cap_term_i = admissify_classical_adem_monomial_one_step_AssumeNoSq0Factors(aPolynomial[i]);
        ClassicalAdemPolynomial_ReplaceTermWithPolynomial(aPolynomial, i, cap_term_i);
        i += cap_term_i.size();
    }
}

