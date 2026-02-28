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

