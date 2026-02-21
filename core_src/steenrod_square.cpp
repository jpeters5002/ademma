#include "steenrod_square.hpp"

#include <string>

std::string ademma_core::SteenrodSquareDegree_ToString(SteenrodSquareDegree aValue)
{
    return "Sq^" + std::to_string(aValue);
}

bool ademma_core::SteenrodSquareDegree_IsPairAdmissible(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight)
{
    return aLeft >= 2 * aRight;
}

