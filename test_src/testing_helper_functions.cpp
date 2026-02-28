#include "testing_helper_functions.hpp"

#include <stdexcept>

ademma_core::ClassicalAdemPolynomial testing::classical_polynomial_from_r_motivic_polynomial(const ademma_core::RMotivicAdemPolynomial& aRMPolynomial)
{
    using namespace ademma_core;
    if (!RMotivicAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(aRMPolynomial))
    {
        throw std::runtime_error("Error: testing::classical_polynomial_from_r_motivic_polynomial called with non-admissible input polynomial");
    }
    ClassicalAdemPolynomial cl_poly {};
    for (size_t i = 0; i < aRMPolynomial.size(); i++)
    {
        const RMotivicAdemMonomial& rm_monomial = aRMPolynomial[i];
        ClassicalAdemMonomial cl_monomial {};
        for (size_t j = 0; j < rm_monomial.size(); j++)
        {
            RMotivicAdemMonomialFactor factor = rm_monomial[j];
            RMotivicAdemMonomialFactor_Type type = RMotivicAdemMonomialFactor_GetType(factor);
            if (type == RMotivicAdemMonomialFactor_Type::cRho)
            {
                goto skip_cl_poly_push_back;
            }
            else if (type == RMotivicAdemMonomialFactor_Type::cTau)
            {
                continue;
            }
            else
            {
                cl_monomial.push_back((SteenrodSquareDegree)factor);
            }
        }
        cl_poly.push_back(cl_monomial);
skip_cl_poly_push_back:
        continue;
    }
    return cl_poly;
}

int testing::two_to_power(int n)
{
    int result = 1;
    for (size_t i = 0; i < n; i++)
    {
        result *= 2;
    }
    return result;
}

