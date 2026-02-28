#include "test_sq2tothen_tothe2nplus2.hpp"

#include "testing_helper_functions.hpp"

#include "classical_adem_math.hpp"
#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_monomial.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "steenrod_square.hpp"

#include <iostream>
#include <assert.h>

#define LAST_N_CASE_TO_TEST_CL 3
#define LAST_N_CASE_TO_TEST_RM 3

int test_sq2tothen_tothe2nplus2()
{
    using namespace ademma_core;
    bool success = true;
    assert(LAST_N_CASE_TO_TEST_CL >= LAST_N_CASE_TO_TEST_RM);
    for (size_t n = 0; n <= LAST_N_CASE_TO_TEST_CL; n++)
    {
        const SteenrodSquareDegree two_to_n = testing::two_to_power(n);
        const int power = 2 * n + 2;
        bool expected_result;

        // cl
        ClassicalAdemMonomial cam {};
        for (size_t z = 0; z < power; z++)
        {
            cam.push_back(two_to_n);
        }
        ClassicalAdemPolynomial cap = classical_adem_math::admissify_classical_adem_monomial(cam);
        expected_result = (cap.size() == 0);
        if (!expected_result)
        {
            std::cerr << "Classical - expected result: 0; Calculation: " << ClassicalAdemMonomial_ToString(cam) << " -> " << ClassicalAdemPolynomial_ToString(cap) << std::endl;
            success = false;
        }

        // rm
        if (n <= LAST_N_CASE_TO_TEST_RM)
        {
            RMotivicAdemMonomial rmam {};
            for (size_t z = 0; z < power; z++)
            {
                rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(two_to_n));
            }
            RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(rmam);
            expected_result = (rmap.size() == 0);
            if (!expected_result)
            {
                std::cerr << "R-Motivic - expected result: 0; Calculation: " << RMotivicAdemMonomial_ToString(rmam) << " -> " << RMotivicAdemPolynomial_ToString(rmap) << std::endl;
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

