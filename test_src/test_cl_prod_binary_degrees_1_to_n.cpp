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

#define LAST_N_CASE_TO_TEST 15

int test_cl_prod_binary_degrees_1_to_n()
{
    using namespace ademma_core;
    bool success = true;
    for (size_t n = 1; n <= LAST_N_CASE_TO_TEST; n++)
    {
        const int expected_degree = testing::two_to_power(n + 1) - 1;
        ClassicalAdemMonomial cam {};
        for (size_t i = 0; i <= n; i++)
        {
            const int two_to_i = testing::two_to_power(i);
            cam.push_back((SteenrodSquareDegree)two_to_i);
        }
        ClassicalAdemPolynomial cap = classical_adem_math::admissify_classical_adem_monomial(cam);
        bool equal_to_expected = (cap.size() == 1 && cap[0].size() == 1 && cap[0][0] == expected_degree);
        if (!equal_to_expected)
        {
            std::cerr << "test_cl_prod_square_degrees_1_to_n did not give the expected result for n = " << std::to_string(n) << "; calculation: " << ClassicalAdemMonomial_ToString(cam) << " -> " << ClassicalAdemPolynomial_ToString(cap) << std::endl;
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

