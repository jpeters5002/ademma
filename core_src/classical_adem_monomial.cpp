#include "classical_adem_monomial.hpp"

std::string ademma_core::ClassicalAdemMonomial_ToString(const ClassicalAdemMonomial& aValue)
{
    std::string outStr = "";
    for (size_t i = 0; i < aValue.size(); i++)
    {
        outStr += SteenrodSquareDegree_ToString(aValue[i]);
    }
    return outStr;
}

