#include <iostream>

#include "math.hpp"
#include "classical_adem_polynomial.hpp"

#include <stdexcept>


int main (int argc, char** argv)
{
    std::cout << "running Sq^6Sq^7 admissify..." << std::endl;
    ademma_core::ClassicalAdemMonomial cam {};
    cam.push_back(6);
    cam.push_back(7);
    ademma_core::ClassicalAdemPolynomial cap {};
    cap = ademma_core::math::admissify_classical_adem_monomial(cam);
    std::cout << ademma_core::ClassicalAdemPolynomial_ToString(cap) << std::endl;
    return 0;
}

// PRIVATE FUNCTIONS DECLARATION

typedef unsigned int ademma_uint_t;

namespace ademma_core::math
{
ademma_uint_t factorial_twos_count(ademma_uint_t aBase);
bool f2_choose(ademma_uint_t aTop, ademma_uint_t aBottom);
ClassicalAdemPolynomial admissify_two_factor_classical_adem_monomial(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight);
}

// PUBLIC FUNCTIONS

ademma_core::ClassicalAdemPolynomial ademma_core::math::admissify_classical_adem_monomial(const ClassicalAdemMonomial& aMonomial)
{
    if (aMonomial.size() > 2)
    {
        throw std::runtime_error("admissify_classical_adem_monomial not implemented for monomials of more than 2 terms");
    }
    ClassicalAdemPolynomial cap {};
    if (aMonomial.size() == 1)
    {
        cap.push_back(aMonomial);
        return cap;
    }
    for (size_t monomial_i = 1; monomial_i < aMonomial.size(); monomial_i++)
    {
        SteenrodSquareDegree term1 = aMonomial[monomial_i - 1];
        SteenrodSquareDegree term2 = aMonomial[monomial_i];
        // TODO: following line needs changed when handling more than 2 factors
        cap = admissify_two_factor_classical_adem_monomial(term1, term2);
    }
    return cap;
}

// PRIVATE FUNCTIONS DEFINITION

ademma_uint_t ademma_core::math::factorial_twos_count(ademma_uint_t aBase)
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

bool ademma_core::math::f2_choose(ademma_uint_t aTop, ademma_uint_t aBottom)
{
    if (aTop < aBottom)
    {
        throw std::runtime_error("f2_choose given aTop is less than given aBottom when it must be >=");
    }
    ademma_uint_t numeratorTwos = factorial_twos_count(aTop);
    ademma_uint_t denominatorTwos = factorial_twos_count(aBottom) + factorial_twos_count(aTop - aBottom);
    return numeratorTwos <= denominatorTwos;
}

ademma_core::ClassicalAdemPolynomial ademma_core::math::admissify_two_factor_classical_adem_monomial(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight)
{
    ClassicalAdemPolynomial cap {};
    if (aLeft >= 2 * aRight)
    {
        // already admissable
        ClassicalAdemMonomial cam {};
        cam.push_back(aLeft);
        cam.push_back(aRight);
        cap.push_back(cam);
    }
    else
    {
        for (int k = 0; k <= aLeft / 2; k++)
        {
            ademma_uint_t chooseTop = aRight - k - 1;
            ademma_uint_t chooseBottom = aLeft - 2 * k;
            if (f2_choose(chooseTop, chooseBottom))
            {
                ClassicalAdemMonomial cam {};
                cam.push_back(aLeft + aRight - k);
                cam.push_back(k);
                cap.push_back(cam);
            }
        }
    }
    return cap;
}

