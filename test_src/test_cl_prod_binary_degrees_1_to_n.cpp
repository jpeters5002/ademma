#include "test_cl_prod_binary_degrees_1_to_n.hpp"

#include "testing_helper_functions.hpp"

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

#define LAST_N_CASE_TO_TEST_CL 15
#define LAST_N_CASE_TO_TEST_CM 9
#define LAST_N_CASE_TO_TEST_RM 6

int test_cl_prod_binary_degrees_1_to_n()
{
    using namespace ademma_core;
    bool success = true;
    assert(LAST_N_CASE_TO_TEST_CL >= LAST_N_CASE_TO_TEST_RM);
    assert(LAST_N_CASE_TO_TEST_CL >= LAST_N_CASE_TO_TEST_CM);
    assert(LAST_N_CASE_TO_TEST_CM >= LAST_N_CASE_TO_TEST_RM);
    for (size_t n = 1; n <= LAST_N_CASE_TO_TEST_CL; n++)
    {
        const int expected_degree = testing::two_to_power(n + 1) - 1;
        ClassicalAdemMonomial cam {};
        RMotivicAdemMonomial rmam {};
        CMotivicAdemMonomial cmam {};
        for (size_t i = 0; i <= n; i++)
        {
            const int two_to_i = testing::two_to_power(i);
            cam.push_back((SteenrodSquareDegree)two_to_i);
            if (n <= LAST_N_CASE_TO_TEST_CM)
            {
                cmam.push_back(CMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(two_to_i));
            }
            if (n <= LAST_N_CASE_TO_TEST_RM)
            {
                rmam.push_back(RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(two_to_i));
            }
        }
        CMotivicAdemPolynomial cmap = c_motivic_adem_math::admissify_c_motivic_adem_monomial(cmam);
        RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(rmam);
        int poly_results;
        ClassicalAdemPolynomial cap[3];
        cap[0] = classical_adem_math::admissify_classical_adem_monomial(cam);

        if (n <= LAST_N_CASE_TO_TEST_RM)
        {
            cap[1] = testing::classical_polynomial_from_c_motivic_polynomial(cmap);
            cap[2] = testing::classical_polynomial_from_r_motivic_polynomial(rmap);
            poly_results = 3;
        }
        else if (n <= LAST_N_CASE_TO_TEST_CM)
        {
            cap[1] = testing::classical_polynomial_from_c_motivic_polynomial(cmap);
            poly_results = 2;
        }
        else
        {
            poly_results = 1;
        }

        for (size_t z = 0; z < poly_results; z++)
        {
            bool equal_to_expected = (cap[z].size() == 1 && cap[z][0].size() == 1 && cap[z][0][0] == expected_degree);
            if (!equal_to_expected)
            {
                std::string conversion_details;
                std::string calculation_input;
                std::string calculation_result = ClassicalAdemPolynomial_ToString(cap[z]);
                if (z == 0)
                { conversion_details = " "; calculation_input = ClassicalAdemMonomial_ToString(cam); }
                else if (z == 1)
                { conversion_details = " (c-motivic case turned classical after calculation)"; calculation_input = CMotivicAdemMonomial_ToString(cmam); }
                else
                { conversion_details = " (r-motivic case turned classical after calculation)"; calculation_input = RMotivicAdemMonomial_ToString(rmam); }
                std::cerr << "test_cl_prod_square_degrees_1_to_n" << conversion_details << " did not give the expected result for n = " << std::to_string(n) << "; calculation: " << calculation_input << " -> " << calculation_result << std::endl;
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

