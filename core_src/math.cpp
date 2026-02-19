#include <stdio.h>

int main (int argc, char** argv)
{
    printf("hello world\n");
    return 0;
}

// everything above here is garbage for now

#include "math.hpp"

#include <exception>

// PRIVATE FUNCTIONS DECLARATION

typedef unsigned int ademma_uint_t;

namespace ademma_core::math
{
ademma_uint_t factorial_twos_count(ademma_uint_t aBase);
bool f2_choose(SteenrodSquareOrder aTop, SteenrodSquareOrder aBottom);
}

// PUBLIC FUNCTIONS

ClassicalAdemPolynomial ademma_core::math::admissify_classical_adem_monomial(const ClassicalAdemMonomial& aMonomial)
{
    if (aMonomial.size() > 2)
    {
        throw std::runtime_error("admissify_classical_adem_monomial not implemented for monomials of more than 2 terms");
    }
    ClassicalAdemPolynomial cap {};
    if (aMonomial.size() == 1)
    {
        cap.push_back(aMonomial[0]);
        return cap;
    }
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        SteenrodSquareOrder term1 = aMonomial[i - 1];
        SteenrodSquareOrder term2 = aMonomial[i];

    }
}

// PRIVATE FUNCTIONS DEFINITION

ademma_uint_t ademma::math::factorial_twos_count(ademma_uint_t aBase)
{
    ademma_uint_t runningCount = 0;
    ademma_uint_t lastCount = aBase;
    for (;;)
    {
        lastCount = lastCount / 2;
        if (lastCount == 0)
        {
            break;
        }
        runningCount += lastCount;
    }
    return runningCount;
}

bool ademma::math::f2_choose(SteenrodSquareOrder aTop, SteenrodSquareOrder aBottom)
{

}

