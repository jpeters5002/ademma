#include <iostream>

#include "classical_adem_math.hpp"
#include "classical_adem_polynomial.hpp"
#include "classical_adem_monomial.hpp"
#include "parsing_info.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_monomial.hpp"

int handle_classical()
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

int handle_r_motivic()
{
    using namespace ademma_core;
    std::string user_input_r_motivic_adem_monomial {};
    std::cout << "Enter R-motivic adem monomial (form eg: Sq^1\\tau\\rho^2Sq^2)" << std::endl;
    ParsingInfo parsing_info;
    std::cin >> parsing_info.mStringToParse;

    RMotivicAdemMonomial user_input_rmam = RMotivicAdemMonomial_FromString(parsing_info);
    if (parsing_info.mErrorInfo.mIsError)
    {
        std::cout << parsing_info.GetFullErrorString() << std::endl;
        return 1;
    }
    RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(user_input_rmam);
    std::cout << RMotivicAdemPolynomial_ToString(rmap) << std::endl;
    return 0;
}

int main (int argc, char** argv)
{
    return handle_r_motivic();
    //return handle_classical();
}

