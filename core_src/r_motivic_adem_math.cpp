#include "r_motivic_adem_math.hpp"

#include "general_adem_math.hpp"
#include "r_motivic_adem_polynomial.hpp"

#include <stdexcept>

// PRIVATE FUNCTIONS DECLARATION

namespace ademma_core::r_motivic_adem_math
{
RMotivicAdemPolynomial admissify_two_factor_r_motivic_adem_monomial(RMotivicAdemMonomialFactor aLeft, RMotivicAdemMonomialFactor aRight);
RMotivicAdemPolynomial admissify_r_motivic_adem_monomial_one_step_AssumeNoSq0Factors(const RMotivicAdemMonomial& aMonomial);
void admissify_r_motivic_adem_polynomial_one_step_AssumeNoSq0Factors(RMotivicAdemPolynomial& aPolynomial);
}

// PUBLIC FUNCTIONS

ademma_core::RMotivicAdemPolynomial ademma_core::r_motivic_adem_math::admissify_r_motivic_adem_monomial(const RMotivicAdemMonomial& aMonomial)
{
    RMotivicAdemPolynomial rmapOut {};
    rmapOut.push_back(aMonomial);
    RMotivicAdemMonomial_EliminateAllPower0Taus(rmapOut[0]);
    for (;;)
    {
        RMotivicAdemPolynomial_EliminateAllSq0Factors(rmapOut);
        RMotivicAdemPolynomial_ShoveRhoLeft(rmapOut);
        RMotivicAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors_AssumeRhoLeft(rmapOut);
        if (RMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(rmapOut))
        {
            break;
        }
        admissify_r_motivic_adem_polynomial_one_step_AssumeNoSq0Factors(rmapOut);
    }
    return rmapOut;
}

// PRIVATE FUNCTIONS DEFINITION

/*
ademma_core::RMotivicAdemPolynomial ademma_core::r_motivic_adem_math::admissify_two_factor_r_motivic_adem_monomial(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight)
{
    RMotivicAdemPolynomial cap {};
    if (SteenrodSquareDegree_IsPairAdmissible(aLeft, aRight))
    {
        RMotivicAdemMonomial cam {};
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
            if (chooseTop >= chooseBottom && f2_choose(chooseTop, chooseBottom))
            {
                RMotivicAdemMonomial cam {};
                cam.push_back(aLeft + aRight - k);
                cam.push_back(k);
                cap.push_back(cam);
            }
        }
    }
    return cap;
}

ademma_core::RMotivicAdemPolynomial ademma_core::r_motivic_adem_math::admissify_r_motivic_adem_monomial_one_step_AssumeNoSq0Factors(const RMotivicAdemMonomial& aMonomial)
{
    RMotivicAdemPolynomial capOut {};
    if (RMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aMonomial))
    {
        capOut.push_back(aMonomial);
        return capOut;
    }
    RMotivicAdemMonomial left_leftover_monomial {};
    RMotivicAdemMonomial right_leftover_monomial {};
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        SteenrodSquareDegree left_degree = aMonomial[i - 1];
        SteenrodSquareDegree right_degree = aMonomial[i];
        if (SteenrodSquareDegree_IsPairAdmissible(left_degree, right_degree))
        {
            continue;
        }
        capOut = admissify_two_factor_r_motivic_adem_monomial(aMonomial[i - 1], aMonomial [i]);
        for (size_t left_j = 0; left_j < i - 1; left_j++)
        {
            left_leftover_monomial.push_back(aMonomial[left_j]);
        }
        for (size_t right_j = i + 1; right_j < aMonomial.size(); right_j++)
        {
            right_leftover_monomial.push_back(aMonomial[right_j]);
        }
        RMotivicAdemPolynomial_MultiplyLeftMonomial(left_leftover_monomial, capOut);
        RMotivicAdemPolynomial_MultiplyRightMonomial(capOut, right_leftover_monomial);
        return capOut;
    }
    throw std::runtime_error("no inadmissible pair found within monomial that was checked to have an inadmissible pair somewhere");
}

void ademma_core::r_motivic_adem_math::admissify_r_motivic_adem_polynomial_one_step_AssumeNoSq0Factors(RMotivicAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size();)
    {
        if (RMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            i++;
            continue;
        }
        RMotivicAdemPolynomial cap_term_i = admissify_r_motivic_adem_monomial_one_step_AssumeNoSq0Factors(aPolynomial[i]);
        RMotivicAdemPolynomial_ReplaceTermWithPolynomial(aPolynomial, i, cap_term_i);
        i += cap_term_i.size() - 1;
    }
}
*/
