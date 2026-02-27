#include "test_cl_prod_binary_degrees_1_to_n.hpp"

#include "testing_helper_functions.hpp"

#include "classical_adem_math.hpp"
#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_monomial.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "steenrod_square.hpp"

#include <iostream>

#define LAST_N_CASE_TO_TEST_CL 15
#define LAST_N_CASE_TO_TEST_RM 6

int test_cl_prod_binary_degrees_1_to_n()
{
    using namespace ademma_core;
    bool success = true;
    for (size_t n = 1; n <= LAST_N_CASE_TO_TEST_CL; n++)
    {
        const int expected_degree = testing::two_to_power(n + 1) - 1;
        ClassicalAdemMonomial cam {};
        RMotivicAdemMonomial rmam {};
        for (size_t i = 0; i <= n; i++)
        {
            const int two_to_i = testing::two_to_power(i);
            cam.push_back((SteenrodSquareDegree)two_to_i);
            if (n <= LAST_N_CASE_TO_TEST_RM)
            {
                rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(two_to_i));
            }
        }
        RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(rmam);
        int poly_results;
        ClassicalAdemPolynomial cap[2];
        if (n <= LAST_N_CASE_TO_TEST_RM)
        {
            cap[0] = classical_adem_math::admissify_classical_adem_monomial(cam);
            cap[1] = testing::classical_polynomial_from_r_motivic_polynomial(rmap);
            poly_results = 2;
        }
        else
        {
            cap[0] = classical_adem_math::admissify_classical_adem_monomial(cam);
            poly_results = 1;
        }

        for (size_t z = 0; z < poly_results; z++)
        {
            bool equal_to_expected = (cap[z].size() == 1 && cap[z][0].size() == 1 && cap[z][0][0] == expected_degree);
            if (!equal_to_expected)
            {
                std::cerr << "test_cl_prod_square_degrees_1_to_n" << ((z == 0) ? "" : " (r-motivic case turned classical after calculation)") << " did not give the expected result for n = " << std::to_string(n) << "; calculation: " << ((z == 0) ? ClassicalAdemMonomial_ToString(cam) : RMotivicAdemMonomial_ToString(rmam)) << " -> " << ((z == 0) ? ClassicalAdemPolynomial_ToString(cap[z]) : RMotivicAdemPolynomial_ToString(rmap)) << std::endl;
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

