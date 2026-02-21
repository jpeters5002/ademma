#ifndef CORE_INCLUDE_STEENROD_SQUARE_HPP
#define CORE_INCLUDE_STEENROD_SQUARE_HPP

#include <string>

namespace ademma_core
{
typedef int SteenrodSquareDegree;
std::string SteenrodSquareDegree_ToString(SteenrodSquareDegree aValue);

bool SteenrodSquareDegree_IsPairAdmissible(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight);
}

#endif // CORE_INCLUDE_STEENROD_SQUARE_HPP

