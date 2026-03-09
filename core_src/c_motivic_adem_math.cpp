#include "c_motivic_adem_math.hpp"

#include "general_adem_math.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "debug_output.hpp"

#include <stdexcept>

#include <assert.h>

// PRIVATE FUNCTIONS DECLARATION

namespace ademma_core { namespace c_motivic_adem_math
{
CMotivicAdemPolynomial admissify_two_factor_c_motivic_adem_monomial_AssumeNoSq0Factors(CMotivicAdemMonomialFactor aLeft, CMotivicAdemMonomialFactor aRight);
CMotivicAdemPolynomial admissify_c_motivic_adem_monomial_one_step_AssumeNoSq0Factors(const CMotivicAdemMonomial& aMonomial);
void admissify_c_motivic_adem_polynomial_one_step_AssumeNoSq0Factors(CMotivicAdemPolynomial& aPolynomial);
}}

// PUBLIC FUNCTIONS

ademma_core::CMotivicAdemPolynomial ademma_core::c_motivic_adem_math::admissify_c_motivic_adem_polynomial(const CMotivicAdemPolynomial& aPolynomial)
{
    CMotivicAdemPolynomial cmapOut = aPolynomial;
    for (;;)
    {
        CMotivicAdemPolynomial_EliminateAllSq0Factors(cmapOut);
        CMotivicAdemPolynomial_ShoveTauLeft(cmapOut);
        CMotivicAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors_AssumeTauLeft(cmapOut);
        if (CMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(cmapOut))
        {
            break;
        }
        admissify_c_motivic_adem_polynomial_one_step_AssumeNoSq0Factors(cmapOut);
    }
    return cmapOut;
}

ademma_core::CMotivicAdemPolynomial ademma_core::c_motivic_adem_math::admissify_c_motivic_adem_monomial(const CMotivicAdemMonomial& aMonomial)
{
    CMotivicAdemPolynomial cmap {};
    cmap.push_back(aMonomial);
    return admissify_c_motivic_adem_polynomial(cmap);
}

// PRIVATE FUNCTIONS DEFINITION

ademma_core::CMotivicAdemPolynomial ademma_core::c_motivic_adem_math::admissify_two_factor_c_motivic_adem_monomial_AssumeNoSq0Factors(CMotivicAdemMonomialFactor aLeft, CMotivicAdemMonomialFactor aRight)
{
    DEBUG_PRINT("c_mot_math::admissify_two_factor... called with: " + CMotivicAdemMonomialFactor_ToString(aLeft) + ", " + CMotivicAdemMonomialFactor_ToString(aRight));
    CMotivicAdemPolynomial cmap {};
    if (CMotivicAdemMonomialFactor_IsPairAdmissible(aLeft, aRight))
    {
        CMotivicAdemMonomial cmam {};
        cmam.push_back(aLeft);
        cmam.push_back(aRight);
        cmap.push_back(cmam);
    }
    else
    {
        CMotivicAdemMonomialFactor_Type left_type = CMotivicAdemMonomialFactor_GetType(aLeft);
        CMotivicAdemMonomialFactor_Type right_type = CMotivicAdemMonomialFactor_GetType(aRight);
        if (right_type == CMotivicAdemMonomialFactor_Type::cTau)
        {
            if (left_type == CMotivicAdemMonomialFactor_Type::cTau)
            {
                // tau^a * tau^b
                int left_power = CMotivicAdemMonomialFactor_GetPower_AssumeTau(aLeft);
                int right_power = CMotivicAdemMonomialFactor_GetPower_AssumeTau(aRight);
                CMotivicAdemMonomialFactor combined = CMotivicAdemMonomialFactor_CreateTau(left_power + right_power);
                CMotivicAdemMonomial cmam {};
                cmam.push_back(combined);
                cmap.push_back(cmam);
            }
            else
            {
                // Sq^a * tau^b
                assert(type_left == CMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree);
                CMotivicAdemMonomial cmam {};
                cmam.push_back(aRight);
                cmam.push_back(aLeft);
                cmap.push_back(cmam);
            }
        }
        else
        {
            assert(right_type == CMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree);
            if (left_type == CMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree)
            {
                // Sq^a * Sq^b
                const CMotivicAdemMonomialFactor tau1 = CMotivicAdemMonomialFactor_CreateTau(1);
                const bool left_is_even = (aLeft % 2 == 0);
                const bool right_is_even = (aRight % 2 == 0);
                for (int k = 0; k <= aLeft / 2; k++)
                {
                    ademma_uint_t choose_top = aRight - k - 1;
                    ademma_uint_t choose_bottom = aLeft - 2 * k;
                    if (choose_top >= choose_bottom && ademma_core::general_adem_math::f2_choose(choose_top, choose_bottom))
                    {
                        const bool k_is_odd = (k % 2 == 1);
                        CMotivicAdemMonomial cmam {};
                        if (left_is_even && right_is_even && k_is_odd)
                        {
                            cmam.push_back(tau1);
                        }
                        cmam.push_back(aLeft + aRight - k);
                        cmam.push_back(k);
                        cmap.push_back(cmam);
                    }
                }
            }
            else
            {
                // tau^a * Sq^b but we're not admissible...?
                throw std::runtime_error("unreachable code: logically impossible to reach this code (tau^a * Sq^b somehow considered inadmissible)");
            }
        }
    }
    DEBUG_PRINT("c_mot_math::admissify_two_factor... done:        " + CMotivicAdemPolynomial_ToString(cmap));
    return cmap;
}

ademma_core::CMotivicAdemPolynomial ademma_core::c_motivic_adem_math::admissify_c_motivic_adem_monomial_one_step_AssumeNoSq0Factors(const CMotivicAdemMonomial& aMonomial)
{
    CMotivicAdemPolynomial capOut {};
    if (CMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aMonomial))
    {
        capOut.push_back(aMonomial);
        return capOut;
    }
    CMotivicAdemMonomial left_leftover_monomial {};
    CMotivicAdemMonomial right_leftover_monomial {};
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        CMotivicAdemMonomialFactor left_factor = aMonomial[i - 1];
        CMotivicAdemMonomialFactor right_factor = aMonomial[i];
        if (CMotivicAdemMonomialFactor_IsPairAdmissible(left_factor, right_factor))
        {
            continue;
        }
        capOut = admissify_two_factor_c_motivic_adem_monomial_AssumeNoSq0Factors(aMonomial[i - 1], aMonomial[i]);
        for (size_t left_j = 0; left_j < i - 1; left_j++)
        {
            left_leftover_monomial.push_back(aMonomial[left_j]);
        }
        for (size_t right_j = i + 1; right_j < aMonomial.size(); right_j++)
        {
            right_leftover_monomial.push_back(aMonomial[right_j]);
        }
        CMotivicAdemPolynomial_MultiplyLeftMonomial(left_leftover_monomial, capOut);
        CMotivicAdemPolynomial_MultiplyRightMonomial(capOut, right_leftover_monomial);
        return capOut;
    }
    throw std::runtime_error("no inadmissible pair found within monomial that was checked to have an inadmissible pair somewhere: " + CMotivicAdemMonomial_ToString(aMonomial));
}

void ademma_core::c_motivic_adem_math::admissify_c_motivic_adem_polynomial_one_step_AssumeNoSq0Factors(CMotivicAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size();)
    {
        if (CMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            i++;
            continue;
        }
        CMotivicAdemPolynomial cmap_term_i = admissify_c_motivic_adem_monomial_one_step_AssumeNoSq0Factors(aPolynomial[i]);
        CMotivicAdemPolynomial_ReplaceTermWithPolynomial(aPolynomial, i, cmap_term_i);
        i += cmap_term_i.size();
    }
}

