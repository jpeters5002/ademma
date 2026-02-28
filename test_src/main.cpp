#include <iostream>

#include "test_cl_prod_binary_degrees_1_to_n.hpp"
#include "test_sq1_sq2n.hpp"
#include "test_sq2nminus1_sqn.hpp"
#include "test_sq2tothen_tothe2nplus2.hpp"

#define SIZEOF_LUT(lut) (sizeof(lut) / sizeof(lut[0]))

struct test_function_t
{
    int (*fn)();
    const char* name;
};

#define TEST_FUNCTION(fn) {fn, #fn}

static test_function_t test_functions[] = {
    TEST_FUNCTION(test_cl_prod_binary_degrees_1_to_n),
    TEST_FUNCTION(test_sq1_sq2n),
    TEST_FUNCTION(test_sq2nminus1_sqn),
    TEST_FUNCTION(test_sq2tothen_tothe2nplus2),
};

int main(int argc, char** argv)
{
    int out_val = 0;
    for (size_t i = 0; i < SIZEOF_LUT(test_functions); i++)
    {
        if (test_functions[i].fn() != 0)
        {
            out_val = 1;
        }
    }
    if (out_val != 0)
    {
        std::cout << "TESTS FAILED" << std::endl;
    }
    else
    {
        std::cout << "TESTS PASSED" << std::endl;
    }
    return out_val;
}

