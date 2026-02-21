#include <iostream>

#include "classical_adem_math.hpp"

int main (int argc, char** argv)
{
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
}

