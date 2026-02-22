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
    ClassicalAdemPolynomial cap = math::admissify_classical_adem_monomial(user_input_cam);
    std::cout << ClassicalAdemPolynomial_ToString(cap) << std::endl;
    return 0;
    /*
    //
    ademma_core::ClassicalAdemMonomial cam {};
    cam.push_back(6);
    cam.push_back(7);
    ademma_core::ClassicalAdemPolynomial cap {};
    std::cout << "running " << ademma_core::ClassicalAdemMonomial_ToString(cam) << " admissify..." << std::endl;
    cap = ademma_core::math::admissify_classical_adem_monomial(cam);
    std::cout << ademma_core::ClassicalAdemPolynomial_ToString(cap) << std::endl;
    //
    cam.clear();
    cap.clear();
    cam.push_back(6);
    cam.push_back(7);
    cam.push_back(4);
    std::cout << "running " << ademma_core::ClassicalAdemMonomial_ToString(cam) << " admissify..." << std::endl;
    cap = ademma_core::math::admissify_classical_adem_monomial(cam);
    std::cout << ademma_core::ClassicalAdemPolynomial_ToString(cap) << std::endl;
    //
    cam.clear();
    cap.clear();
    cam.push_back(3);
    cam.push_back(7);
    cam.push_back(10);
    cam.push_back(13);
    std::cout << "running " << ademma_core::ClassicalAdemMonomial_ToString(cam) << " admissify..." << std::endl;
    cap = ademma_core::math::admissify_classical_adem_monomial(cam);
    std::cout << ademma_core::ClassicalAdemPolynomial_ToString(cap) << std::endl;
    //
    cam.clear();
    cap.clear();
    cam.push_back(6);
    cam.push_back(19);
    cam.push_back(14);
    cam.push_back(10);
    cam.push_back(13);
    cam.push_back(14);
    cam.push_back(22);
    std::cout << "running " << ademma_core::ClassicalAdemMonomial_ToString(cam) << " admissify..." << std::endl;
    cap = ademma_core::math::admissify_classical_adem_monomial(cam);
    std::cout << ademma_core::ClassicalAdemPolynomial_ToString(cap) << std::endl;
    return 0;
    */
}

