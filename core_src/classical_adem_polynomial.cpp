#include "classical_adem_polynomial.hpp"

std::string ademma_core::ClassicalAdemPolynomial_ToString(const ClassicalAdemPolynomial& aValue)
{
    std::string outStr = "";
    for (size_t i = 0; i < aValue.size(); i++)
    {
        if (i != 0)
        {
            outStr += " + ";
        }
        outStr += ClassicalAdemMonomial_ToString(aValue[i]);
    }
    return outStr;
}

