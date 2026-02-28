#include "test_rm_two_factor.hpp"

#include "testing_helper_functions.hpp"

#include "classical_adem_math.hpp"
#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_monomial.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "steenrod_square.hpp"

#include <iostream>

#define LARGEST_J_TO_RUN 100

// PRIVATE FUNCTIONS DECLARATION

bool r_motivic_adem_polynomial_ensure_taus_on_exactly_double_odd_terms(const ademma_core::RMotivicAdemPolynomial& aPolynomial);

// PUBLIC FUNCTIONS DEFINITION

int test_rm_two_factor()
{
    using namespace ademma_core;
    bool success = true;
    for (int j = 1; j <= LARGEST_J_TO_RUN; j++)
    {
        for (int i = 1; i < 2 * j; i++)
        {
            if ((i % 2) != (j % 2))
            {
                continue;
            }
            SteenrodSquareDegree left = i;
            SteenrodSquareDegree right = j;

            ClassicalAdemMonomial cam {};
            cam.push_back(left);
            cam.push_back(right);
            ClassicalAdemPolynomial cap = classical_adem_math::admissify_classical_adem_monomial(cam);

            RMotivicAdemMonomial rmam {};
            rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(left));
            rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(right));
            RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(rmam);

            int tau_terms, rho_terms;
            ClassicalAdemPolynomial cap_from_rmap = testing::classical_polynomial_from_r_motivic_polynomial(rmap, &tau_terms, &rho_terms);
            if (rho_terms != 0)
            {
                std::cerr << "Running R-Motivic admissify on " << RMotivicAdemMonomial_ToString(rmam) << " yielded " << RMotivicAdemPolynomial_ToString(rmap) << " which unexpectedly contains at least one term with rho (there are supposed to be 0 when both Sq degrees are odd)" << std::endl;
                success = false;
            }
            bool odd = (i % 2);
            if (odd && tau_terms != 0)
            {
                std::cerr << "Running R-Motivic admissify on " << RMotivicAdemMonomial_ToString(rmam) << " yielded " << RMotivicAdemPolynomial_ToString(rmap) << " which unexpectedly contains at least one term with tau (there are supposed to be 0 when both Sq degrees are odd)" << std::endl;
                success = false;
            }
            if (!ClassicalAdemPolynomial_IsEqualInForm(cap, cap_from_rmap))
            {
                std::cerr << "Running R-Motivic admissify on " << RMotivicAdemMonomial_ToString(rmam) << " yielded " << RMotivicAdemPolynomial_ToString(rmap) << ", while running Classical admissify on " << ClassicalAdemMonomial_ToString(cam) << " yielded " << ClassicalAdemPolynomial_ToString(rmap) << " which were supposed to be the same result" << std::endl;
                success = false;
            }
            if (!odd && !r_motivic_adem_polynomial_ensure_taus_on_exactly_double_odd_terms(rmap))
            {
                // error messages output within r_motivic_adem_polynomial_ensure_taus_on_exactly_double_odd_terms
                success = false;
            }
        }
    }
    if (success)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// PRIVATE FUNCTIONS DEFINITION

bool r_motivic_adem_polynomial_ensure_taus_on_exactly_double_odd_terms(const ademma_core::RMotivicAdemPolynomial& aPolynomial)
{
    using namespace ademma_core;
    bool success = true;
    for (size_t i = 0; i < aPolynomial.size(); i++)
    {
        const RMotivicAdemMonomial& monomial = aPolynomial[i];
        bool contains_tau_factor = false;
        size_t num_odd_square_factors = 0;
        size_t num_even_square_factors = 0;
        for (size_t j = 0; j < monomial.size(); j++)
        {
            RMotivicAdemMonomialFactor factor = monomial[j];
            RMotivicAdemMonomialFactor_Type type = RMotivicAdemMonomialFactor_GetType(factor);
            if (type == RMotivicAdemMonomialFactor_Type::cTau)
            {
                if (RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(factor) > 0)
                {
                    contains_tau_factor = true;
                }
            }
            else if (type == RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree)
            {
                if (factor % 2 == 1)
                {
                    num_odd_square_factors++;
                }
                else
                {
                    num_even_square_factors++;
                }
            }
        }
        if (num_odd_square_factors + num_even_square_factors == 1)
        {
            continue;
        }
        if (num_odd_square_factors + num_even_square_factors != 2)
        {
            std::cerr << "R-Motivic adem polynomial resulting from calculation expected to have two square factors in each term, but term index " << i << " (zero indexed) has " << num_odd_square_factors + num_even_square_factors << "\n\t" << RMotivicAdemPolynomial_ToString(aPolynomial) <<  std::endl;
            success = false;
        }
        else if (num_odd_square_factors == 2 && !contains_tau_factor)
        {
            std::cerr << "R-Motivic adem polynomial resulting from calculation expected to have taus exactly when the term has two odd square degree factors, but term index " << i << " (zero indexed) has two odd square degrees and no tau\n\t" << RMotivicAdemPolynomial_ToString(aPolynomial) <<  std::endl;
            success = false;
        }
        else if (num_odd_square_factors != 2 && contains_tau_factor)
        {
            std::cerr << "R-Motivic adem polynomial resulting from calculation expected to have taus exactly when the term has two odd square degree factors, but term index " << i << " (zero indexed) doesn't have two odd square degrees and does have a tau\n\t" << RMotivicAdemPolynomial_ToString(aPolynomial) <<  std::endl;
            success = false;
        }
    }
    return success;
}

