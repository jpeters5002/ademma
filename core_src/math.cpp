#include <iostream>

#include "math.hpp"
#include "classical_adem_polynomial.hpp"

#include <stdexcept>


int main (int argc, char** argv)
{
    // Sq6Sq7
    std::cout << "running Sq^6Sq^7 admissify..." << std::endl;
    ademma_core::ClassicalAdemMonomial cam {};
    cam.push_back(6);
    cam.push_back(7);
    ademma_core::ClassicalAdemPolynomial cap {};
    cap = ademma_core::math::admissify_classical_adem_monomial(cam);
    std::cout << ademma_core::ClassicalAdemPolynomial_ToString(cap) << std::endl;
    // Sq6Sq7Sq4
    std::cout << "running Sq^6Sq^7Sq^4 admissify..." << std::endl;
    ademma_core::ClassicalAdemMonomial cam2 {};
    cam2.push_back(6);
    cam2.push_back(7);
    cam2.push_back(4);
    ademma_core::ClassicalAdemPolynomial cap2 {};
    cap2 = ademma_core::math::admissify_classical_adem_monomial(cam2);
    std::cout << ademma_core::ClassicalAdemPolynomial_ToString(cap2) << std::endl;
    return 0;
}

// PRIVATE FUNCTIONS DECLARATION

typedef unsigned int ademma_uint_t;

namespace ademma_core::math
{
ademma_uint_t factorial_twos_count(ademma_uint_t aBase);
bool f2_choose(ademma_uint_t aTop, ademma_uint_t aBottom);
ClassicalAdemPolynomial admissify_two_factor_classical_adem_monomial(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight);
ClassicalAdemPolynomial admissify_classical_adem_monomial_one_step_AssumeNoSq0Factors(const ClassicalAdemMonomial& aMonomial);
void admissify_classical_adem_polynomial_one_step_AssumeNoSq0Factors(ClassicalAdemPolynomial& aPolynomial);
}

// PUBLIC FUNCTIONS

ademma_core::ClassicalAdemPolynomial ademma_core::math::admissify_classical_adem_monomial(const ClassicalAdemMonomial& aMonomial)
{
    ClassicalAdemPolynomial capOut {};
    capOut.push_back(aMonomial);
    for (;;)
    {
        ClassicalAdemPolynomial_EliminateAllSq0Factors(capOut);
        ClassicalAdemPolynomial_CombineLikeTerms_AssumeNoSq0Factors(capOut);
        if (ClassicalAdemPolynomial_IsAdmissible_AssumeNoLikeTerms_AssumeNoSq0Factors(capOut))
        {
            break;
        }
        admissify_classical_adem_polynomial_one_step_AssumeNoSq0Factors(capOut);
    }
    return capOut;
    // Sq10Sq5Sq6Sq4
    // (Sq10Sq5)Sq6Sq4
    // Sq10(Sq5Sq6)Sq4
    // (monomial)(polynomial)(monomial)
    // 
    // P, zoom in on some index of the terms, n, then we need admissify_monomial(P[n]) which can create another polynomial, which inserts itself as part of P at index n
    // fn admissify_one_step(monomial) -> polynomial
    // fn admissify_polynomial_one_step_each_term(polynomial) -> polynomial

    //cap = aMonomial;
    //for (size_t monomial_i = 1; monomial_i < aMonomial.size(); monomial_i++)
    //{
        //SteenrodSquareDegree term1 = aMonomial[monomial_i - 1];
        //SteenrodSquareDegree term2 = aMonomial[monomial_i];
        //// TODO: following line needs changed when handling more than 2 factors
        //cap = admissify_two_factor_classical_adem_monomial(term1, term2);
    //}
    //return cap;
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
        // already admissible
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

ademma_core::ClassicalAdemPolynomial ademma_core::math::admissify_classical_adem_monomial_one_step_AssumeNoSq0Factors(const ClassicalAdemMonomial& aMonomial)
{
    ClassicalAdemPolynomial capOut {};
    if (ClassicalAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aMonomial))
    {
        capOut.push_back(aMonomial);
        return capOut;
    }
    ClassicalAdemMonomial left_leftover_monomial {};
    ClassicalAdemMonomial right_leftover_monomial {};
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        SteenrodSquareDegree left_degree = aMonomial[i - 1];
        SteenrodSquareDegree right_degree = aMonomial[i];
        if (SteenrodSquareDegree_IsPairAdmissible(left_degree, right_degree))
        {
            continue;
        }
        capOut = admissify_two_factor_classical_adem_monomial(aMonomial[i - 1], aMonomial [i]);
        for (size_t left_j = 0; left_j < i - 1; left_j++)
        {
            left_leftover_monomial.push_back(aMonomial[left_j]);
        }
        for (size_t right_j = i + 1; right_j < aMonomial.size(); right_j++)
        {
            right_leftover_monomial.push_back(aMonomial[right_j]);
        }
        ClassicalAdemPolynomial_MultiplyLeftMonomial(left_leftover_monomial, capOut);
        ClassicalAdemPolynomial_MultiplyRightMonomial(capOut, right_leftover_monomial);
        return capOut;
    }
    throw std::runtime_error("no inadmissible pair found within monomial that was checked to have an inadmissible pair somewhere");
}

void ademma_core::math::admissify_classical_adem_polynomial_one_step_AssumeNoSq0Factors(ClassicalAdemPolynomial& aPolynomial)
{
    for (size_t i = 0; i < aPolynomial.size();)
    {
        if (ClassicalAdemMonomial_IsAdmissible_AssumeNoSq0Factors(aPolynomial[i]))
        {
            i++;
            continue;
        }
        ClassicalAdemPolynomial cap_term_i = admissify_classical_adem_monomial_one_step_AssumeNoSq0Factors(aPolynomial[i]);
        ClassicalAdemPolynomial_ReplaceTermWithPolynomial(aPolynomial, i, cap_term_i);
        i += cap_term_i.size() - 1;
    }
}

