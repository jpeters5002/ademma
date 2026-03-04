#include "r_motivic_adem_math.hpp"

#include "general_adem_math.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "debug_output.hpp"

#include <stdexcept>

#include <assert.h>

// PRIVATE FUNCTIONS DECLARATION

namespace ademma_core { namespace r_motivic_adem_math
{
RMotivicAdemPolynomial admissify_two_factor_r_motivic_adem_monomial_AssumeNoSq0Factors(RMotivicAdemMonomialFactor aLeft, RMotivicAdemMonomialFactor aRight, int& aLeftoverRightTaus);
RMotivicAdemPolynomial admissify_r_motivic_adem_monomial_one_step_AssumeNoSq0Factors(const RMotivicAdemMonomial& aMonomial);
void admissify_r_motivic_adem_polynomial_one_step_AssumeNoSq0Factors(RMotivicAdemPolynomial& aPolynomial);
}}

// PUBLIC FUNCTIONS

ademma_core::RMotivicAdemPolynomial ademma_core::r_motivic_adem_math::admissify_r_motivic_adem_polynomial(const RMotivicAdemPolynomial& aPolynomial)
{
    RMotivicAdemPolynomial rmapOut = aPolynomial;
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

ademma_core::RMotivicAdemPolynomial ademma_core::r_motivic_adem_math::admissify_r_motivic_adem_monomial(const RMotivicAdemMonomial& aMonomial)
{
    RMotivicAdemPolynomial rmap {};
    rmap.push_back(aMonomial);
    return admissify_r_motivic_adem_polynomial(rmap);
}

// PRIVATE FUNCTIONS DEFINITION

ademma_core::RMotivicAdemPolynomial ademma_core::r_motivic_adem_math::admissify_two_factor_r_motivic_adem_monomial_AssumeNoSq0Factors(RMotivicAdemMonomialFactor aLeft, RMotivicAdemMonomialFactor aRight, int& aLeftoverRightTaus)
{
    DEBUG_PRINT("r_mot_math::admissify_two_factor... called with: " + RMotivicAdemMonomialFactor_ToString(aLeft) + ", " + RMotivicAdemMonomialFactor_ToString(aRight));
    aLeftoverRightTaus = 0;
    RMotivicAdemPolynomial rmap {};
    if (RMotivicAdemMonomialFactor_IsPairAdmissible(aLeft, aRight))
    {
        RMotivicAdemMonomial rmam {};
        rmam.push_back(aLeft);
        rmam.push_back(aRight);
        rmap.push_back(rmam);
    }
    else
    {
        RMotivicAdemMonomialFactor_Type left_type = RMotivicAdemMonomialFactor_GetType(aLeft);
        RMotivicAdemMonomialFactor_Type right_type = RMotivicAdemMonomialFactor_GetType(aRight);
        if (right_type == RMotivicAdemMonomialFactor_Type::cRho)
        {
            if (left_type == RMotivicAdemMonomialFactor_Type::cRho)
            {
                // rho^a * rho^b
                int left_power = RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(aLeft);
                int right_power = RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(aRight);
                RMotivicAdemMonomialFactor combined = RMotivicAdemMonomialFactor_CreateRho(left_power + right_power);
                RMotivicAdemMonomial rmam {};
                rmam.push_back(combined);
                rmap.push_back(rmam);
            }
            else
            {
                // tau^a * rho^b or Sq^a * rho^b
                RMotivicAdemMonomial rmam {};
                rmam.push_back(aRight);
                rmam.push_back(aLeft);
                rmap.push_back(rmam);
            }
        } // r != rho
        else if (right_type == RMotivicAdemMonomialFactor_Type::cTau)
        {
            if (left_type == RMotivicAdemMonomialFactor_Type::cTau)
            {
                // tau^a * tau^b
                int left_power = RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(aLeft);
                int right_power = RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(aRight);
                RMotivicAdemMonomialFactor combined = RMotivicAdemMonomialFactor_CreateTau(left_power + right_power);
                RMotivicAdemMonomial rmam {};
                rmam.push_back(combined);
                rmap.push_back(rmam);
            }
            else if (left_type == RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree)
            {
                // Sq^a * tau^b
                int right_power = RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(aRight);
                if (right_power == 0)
                {
                    RMotivicAdemMonomial rmam {};
                    rmam.push_back(aLeft);
                    rmap.push_back(rmam);
                }
                else
                {
                    aLeftoverRightTaus = right_power - 1;
                    bool left_degree_even = (((SteenrodSquareDegree)aLeft) % 2 == 0);
                    const RMotivicAdemMonomialFactor rho1 = RMotivicAdemMonomialFactor_CreateRho(1);
                    const RMotivicAdemMonomialFactor tau1 = RMotivicAdemMonomialFactor_CreateTau(1);
                    if (left_degree_even)
                    {
                        RMotivicAdemMonomial rmam {};
                        rmam.push_back(tau1);
                        rmam.push_back(aLeft);
                        rmap.push_back(rmam);
                        rmam.clear();
                        rmam.push_back(rho1);
                        rmam.push_back(tau1);
                        rmam.push_back(aLeft - 1);
                        rmap.push_back(rmam);
                    }
                    else
                    {
                        RMotivicAdemMonomialFactor rho2 = RMotivicAdemMonomialFactor_CreateRho(2);
                        RMotivicAdemMonomial rmam {};
                        rmam.push_back(tau1);
                        rmam.push_back(aLeft);
                        rmap.push_back(rmam);
                        rmam.clear();
                        rmam.push_back(rho1);
                        rmam.push_back(aLeft - 1);
                        rmap.push_back(rmam);
                        if (aLeft > 1)
                        {
                            rmam.clear();
                            rmam.push_back(rho2);
                            rmam.push_back(aLeft - 2);
                            rmap.push_back(rmam);
                        }
                    }
                }
            }
            else
            {
                // rho^a * tau^b but we're not admissible...?
                throw std::runtime_error("unreachable code: logically impossible to reach this code (rho^a * tau^b somehow considered inadmissible)");
            }
        } // r != rho and r != tau => r = steenrod_square_degree
        else
        {
            assert(right_type == RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree);
            if (left_type == RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree)
            {
                // Sq^a * Sq^b
                const RMotivicAdemMonomialFactor rho1 = RMotivicAdemMonomialFactor_CreateRho(1);
                const RMotivicAdemMonomialFactor tau1 = RMotivicAdemMonomialFactor_CreateTau(1);
                bool left_is_even = (aLeft % 2 == 0);
                bool right_is_even = (aRight % 2 == 0);
                if (!left_is_even && !right_is_even)
                {
                    for (int k = 0; k <= aLeft / 2; k++)
                    {
                        ademma_uint_t choose_top = aRight - k - 1;
                        ademma_uint_t choose_bottom = aLeft - 2 * k;
                        if (choose_top >= choose_bottom && ademma_core::general_adem_math::f2_choose(choose_top, choose_bottom))
                        {
                            RMotivicAdemMonomial rmam {};
                            rmam.push_back(aLeft + aRight - k);
                            rmam.push_back(k);
                            rmap.push_back(rmam);
                        }
                    }
                }
                else if (left_is_even && right_is_even)
                {
                    for (int k = 0; k <= aLeft / 2; k++)
                    {
                        ademma_uint_t choose_top = aRight - k - 1;
                        ademma_uint_t choose_bottom = aLeft - 2 * k;
                        if (choose_top >= choose_bottom && ademma_core::general_adem_math::f2_choose(choose_top, choose_bottom))
                        {
                            bool sigma_k = (k % 2 == 1);
                            RMotivicAdemMonomial rmam {};
                            if (sigma_k)
                            {
                                rmam.push_back(tau1);
                            }
                            rmam.push_back(aLeft + aRight - k);
                            rmam.push_back(k);
                            rmap.push_back(rmam);
                        }
                    }
                }
                else if (!left_is_even && right_is_even)
                {
                    for (int k = 0; k <= aLeft / 2; k++)
                    {
                        ademma_uint_t choose_top = aRight - k - 1;
                        ademma_uint_t choose_bottom = aLeft - 2 * k;
                        if (choose_top >= choose_bottom && ademma_core::general_adem_math::f2_choose(choose_top, choose_bottom))
                        {
                            RMotivicAdemMonomial rmam {};
                            rmam.push_back(aLeft + aRight - k);
                            rmam.push_back(k);
                            rmap.push_back(rmam);
                        }
                        choose_top = aRight - k;
                        choose_bottom = aLeft - 2 * k;
                        if (choose_top >= choose_bottom && ademma_core::general_adem_math::f2_choose(choose_top, choose_bottom))
                        {
                            RMotivicAdemMonomial rmam {};
                            rmam.push_back(rho1);
                            rmam.push_back(aLeft + aRight - k - 1);
                            rmam.push_back(k);
                            rmap.push_back(rmam);
                        }
                    }
                }
                else
                {
                    assert(left_is_even && !right_is_even);
                    for (int k = 0; k <= aLeft / 2; k++)
                    {
                        ademma_uint_t choose_top = aRight - k - 1;
                        ademma_uint_t choose_bottom = aLeft - 2 * k;
                        if (choose_top >= choose_bottom && ademma_core::general_adem_math::f2_choose(choose_top, choose_bottom))
                        {
                            RMotivicAdemMonomial rmam {};
                            rmam.push_back(aLeft + aRight - k);
                            rmam.push_back(k);
                            rmap.push_back(rmam);
                        }
                        choose_top = aRight - 1 - k;
                        choose_bottom = aLeft + 1 - 2 * k;
                        if (choose_top >= choose_bottom && ademma_core::general_adem_math::f2_choose(choose_top, choose_bottom))
                        {
                            RMotivicAdemMonomial rmam {};
                            rmam.push_back(rho1);
                            rmam.push_back(aLeft + aRight - k - 1);
                            rmam.push_back(k);
                            rmap.push_back(rmam);
                        }
                    }
                }
            }
            else
            {
                // rho^a * Sq^b or tau^a * Sq^b but we're not admissible...?
                throw std::runtime_error("unreachable code: logically impossible to reach this code (rho^a * Sq^b or tau^a * Sq^b somehow considered inadmissible)");
            }
        }
    }
    DEBUG_PRINT("r_mot_math::admissify_two_factor... done:        " + RMotivicAdemPolynomial_ToString(rmap) + ", " + std::to_string(aLeftoverRightTaus));
    return rmap;
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
        RMotivicAdemMonomialFactor left_factor = aMonomial[i - 1];
        RMotivicAdemMonomialFactor right_factor = aMonomial[i];
        if (RMotivicAdemMonomialFactor_IsPairAdmissible(left_factor, right_factor))
        {
            continue;
        }
        int num_leftover_right_taus;
        capOut = admissify_two_factor_r_motivic_adem_monomial_AssumeNoSq0Factors(aMonomial[i - 1], aMonomial[i], num_leftover_right_taus);
        if (num_leftover_right_taus > 0)
        {
            right_leftover_monomial.insert(right_leftover_monomial.begin(), RMotivicAdemMonomialFactor_CreateTau(num_leftover_right_taus));
        }
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
    throw std::runtime_error("no inadmissible pair found within monomial that was checked to have an inadmissible pair somewhere: " + RMotivicAdemMonomial_ToString(aMonomial));
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
        RMotivicAdemPolynomial rmap_term_i = admissify_r_motivic_adem_monomial_one_step_AssumeNoSq0Factors(aPolynomial[i]);
        RMotivicAdemPolynomial_ReplaceTermWithPolynomial(aPolynomial, i, rmap_term_i);
        i += rmap_term_i.size();
    }
}

