#include <iostream>

#include "classical_adem_math.hpp"
#include "classical_adem_polynomial.hpp"
#include "classical_adem_monomial.hpp"
#include "parsing_info.hpp"

// for now:
// ./ademma
// sq^1sq^3
// -> result
//
// later:
// ./ademma
// sq^1sq^3
// -> assume classical
// -> result
// OR
// ./ademma
// C/cm/rm
// c
// enter classical
// sq^1sq^3
// -> result

int main (int argc, char** argv)
{
    using namespace ademma_core;
    std::string user_input_classical_adem_monomial {};
    std::cout << "Enter classical adem monomial (form Sq^<num1>Sq^<num2>[Sq^<num3>]...)" << std::endl;
    ParsingInfo parsing_info;
    std::cin >> parsing_info.mStringToParse;

    ClassicalAdemMonomial user_input_cam = ClassicalAdemMonomial_FromString(parsing_info);
    if (parsing_info.mErrorInfo.mIsError)
    {
        std::cout << parsing_info.GetFullErrorString() << std::endl;
        return 1;
    }
    ClassicalAdemPolynomial cap = classical_adem_math::admissify_classical_adem_monomial(user_input_cam);
    std::cout << ClassicalAdemPolynomial_ToString(cap) << std::endl;
    return 0;
}

