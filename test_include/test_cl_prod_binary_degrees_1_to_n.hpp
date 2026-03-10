#ifndef TEST_INCLUDE_TEST_CL_PROD_SQUARE_DEGREES_1_TO_N_HPP
#define TEST_INCLUDE_TEST_CL_PROD_SQUARE_DEGREES_1_TO_N_HPP

// in cl, \prod_{i=0}^{n}Sq^{2^{i}} = Sq^{2^{n+1}-1}
// in rm and cm, we can run the same calculation then convert the result to classical to ensure \rho ends up on all the rm terms it should and that cm is generally doing the correct thing

int test_cl_prod_binary_degrees_1_to_n();

#endif // TEST_INCLUDE_TEST_CL_PROD_SQUARE_DEGREES_1_TO_N_HPP

