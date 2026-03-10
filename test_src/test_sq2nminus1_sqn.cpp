#include "test_sq2nminus1_sqn.hpp"

#include "c_motivic_adem_math.hpp"
#include "c_motivic_adem_monomial.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "classical_adem_math.hpp"
#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_monomial.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "steenrod_square.hpp"

#include <iostream>
#include <assert.h>

#define LAST_N_CASE_TO_TEST 3000

int test_sq2nminus1_sqn()
{
    using namespace ademma_core;
    bool success = true;
    for (size_t n = 1; n <= LAST_N_CASE_TO_TEST; n++)
    {
        const SteenrodSquareDegree first_factor = 2 * n - 1;
        const SteenrodSquareDegree second_factor = n;
        bool expected_result;

        // cl
        ClassicalAdemMonomial cam {};
        cam.push_back(first_factor);
        cam.push_back(second_factor);
        ClassicalAdemPolynomial cap = classical_adem_math::admissify_classical_adem_monomial(cam);
        expected_result = (cap.size() == 0);
        if (!expected_result)
        {
            std::cerr << "Classical - expected result: 0; Calculation: " << ClassicalAdemMonomial_ToString(cam) << " -> " << ClassicalAdemPolynomial_ToString(cap) << std::endl;
            success = false;
        }

        // rm
        RMotivicAdemMonomial rmam {};
        rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(first_factor));
        rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(second_factor));
        RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(rmam);
        if (n % 2 == 1)
        {
            expected_result = (rmap.size() == 0);
        }
        else
        {
            expected_result = (rmap.size() == 1 && rmap[0].size() == 3 && rmap[0][0] == RMotivicAdemMonomialFactor_CreateRho(1) && rmap[0][1] == RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(first_factor) && rmap[0][2] == RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(second_factor - 1));
        }
        if (!expected_result)
        {
            RMotivicAdemMonomialFactor rho1 = RMotivicAdemMonomialFactor_CreateRho(1);
            RMotivicAdemMonomialFactor f1 = RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(first_factor);
            RMotivicAdemMonomialFactor f2 = RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(second_factor);
            std::string expected_result_str = ((n % 2 == 1) ? "0" : RMotivicAdemMonomialFactor_ToString(rho1) + RMotivicAdemMonomialFactor_ToString(f1) + RMotivicAdemMonomialFactor_ToString(f2 - 1));
            std::cerr << "R-Motivic - expected result: " << expected_result_str << "; Calculation: " << RMotivicAdemMonomial_ToString(rmam) << " -> " << RMotivicAdemPolynomial_ToString(rmap) << std::endl;
            success = false;
        }

        // cm
        CMotivicAdemMonomial cmam {};
        cmam.push_back(first_factor);
        cmam.push_back(second_factor);
        CMotivicAdemPolynomial cmap = c_motivic_adem_math::admissify_c_motivic_adem_monomial(cmam);
        expected_result = (cmap.size() == 0);
        if (!expected_result)
        {
            std::cerr << "C-Motivic - expected result: 0; Calculation: " << CMotivicAdemMonomial_ToString(cmam) << " -> " << CMotivicAdemPolynomial_ToString(cmap) << std::endl;
            success = false;
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

