#include "test_sq1_sq2n.hpp"

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

#define LAST_N_CASE_TO_TEST 50

int test_sq1_sq2n()
{
    using namespace ademma_core;
    bool success = true;
    for (int n = 1; n <= LAST_N_CASE_TO_TEST; n++)
    {
        const SteenrodSquareDegree second_factor = 2 * n;
        const SteenrodSquareDegree expected_degree = second_factor + 1;
        bool expected_result;

        // cl
        ClassicalAdemMonomial cam {};

        cam.push_back((SteenrodSquareDegree)1);
        cam.push_back(second_factor);
        ClassicalAdemPolynomial cap = classical_adem_math::admissify_classical_adem_monomial(cam);
        expected_result = (cap.size() == 1 && cap[0].size() == 1 && cap[0][0] == expected_degree);
        if (!expected_result)
        {
            std::cerr << "Classical - expected result: " << SteenrodSquareDegree_ToString(expected_degree) << "; Calculation: " << ClassicalAdemMonomial_ToString(cam) << " -> " << ClassicalAdemPolynomial_ToString(cap) << std::endl;
            success = false;
        }

        // rm
        RMotivicAdemMonomial rmam {};
        rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(1));
        rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(second_factor));
        RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(rmam);
        expected_result = (rmap.size() == 1 && rmap[0].size() == 1 && rmap[0][0] == expected_degree);
        if (!expected_result)
        {
            std::cerr << "R-Motivic - expected result: " << SteenrodSquareDegree_ToString(expected_degree) << "; Calculation: " << RMotivicAdemMonomial_ToString(rmam) << " -> " << RMotivicAdemPolynomial_ToString(rmap) << std::endl;
            success = false;
        }

        // cm
        CMotivicAdemMonomial cmam {};
        cmam.push_back(CMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(1));
        cmam.push_back(CMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(second_factor));
        CMotivicAdemPolynomial cmap = c_motivic_adem_math::admissify_c_motivic_adem_monomial(cmam);
        expected_result = (cmap.size() == 1 && cmap[0].size() == 1 && cmap[0][0] == expected_degree);
        if (!expected_result)
        {
            std::cerr << "C-Motivic - expected result: " << SteenrodSquareDegree_ToString(expected_degree) << "; Calculation: " << CMotivicAdemMonomial_ToString(cmam) << " -> " << CMotivicAdemPolynomial_ToString(cmap) << std::endl;
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

