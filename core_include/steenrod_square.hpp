#ifndef CORE_INCLUDE_STEENROD_SQUARE_HPP
#define CORE_INCLUDE_STEENROD_SQUARE_HPP

#include <string>

#include "parsing_info.hpp"

namespace ademma_core
{
typedef int SteenrodSquareDegree;

static constexpr const SteenrodSquareDegree cSteenrodSquareDegree_ERROR_VALUE = -1;

std::string SteenrodSquareDegree_ToString(SteenrodSquareDegree aValue);

SteenrodSquareDegree SteenrodSquareDegree_FromString(ParsingInfo& aParsingInfo);

bool SteenrodSquareDegree_IsPairAdmissible(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight);
}

#endif // CORE_INCLUDE_STEENROD_SQUARE_HPP

