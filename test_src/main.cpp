#include <iostream>

#include "test_aci_input.hpp"
#include "test_cl_prod_binary_degrees_1_to_n.hpp"
#include "test_rm_two_factor.hpp"
#include "test_sq1_sq2n.hpp"
#include "test_sq2nminus1_sqn.hpp"
#include "test_sq2tothen_tothe2nplus2.hpp"

#include "debug_output.hpp"

#define SIZEOF_LUT(lut) (sizeof(lut) / sizeof(lut[0]))

struct test_function_t
{
    int (*fn)();
    const char* name;
};

#define TEST_FUNCTION(fn) {fn, #fn}

static test_function_t test_functions[] = {
    TEST_FUNCTION(test_aci_input),
    TEST_FUNCTION(test_cl_prod_binary_degrees_1_to_n),
    TEST_FUNCTION(test_rm_two_factor),
    TEST_FUNCTION(test_sq1_sq2n),
    TEST_FUNCTION(test_sq2nminus1_sqn),
    TEST_FUNCTION(test_sq2tothen_tothe2nplus2),
};

int main()
{
    int out_val = 0;
    for (size_t i = 0; i < SIZEOF_LUT(test_functions); i++)
    {
        if (test_functions[i].fn() != 0)
        {
            std::cout << "\tFAIL - " << test_functions[i].name << std::endl;
            out_val = 1;
        }
        else
        {
            std::cout << "\tPASS - " << test_functions[i].name << std::endl;
        }
    }
    if (out_val != 0)
    {
        std::cout << std::endl << "TESTS FAILED" << std::endl;
    }
    else
    {
#if DEBUG_OUTPUT
        std::cout << std::endl << "TESTS FAILED (DEBUG_OUTPUT is activated)" << std::endl;
        out_val = 1;
#else
        std::cout << std::endl << "TESTS PASSED" << std::endl;
#endif
    }
    return out_val;
}

