#ifndef CORE_INCLUDE_CLASSICAL_ADEM_MONOMIAL
#define CORE_INCLUDE_CLASSICAL_ADEM_MONOMIAL

#include "steenrod_square.hpp"
#include "parsing_info.hpp"

#include <vector>
#include <string>

namespace ademma_core
{
typedef std::vector<SteenrodSquareDegree> ClassicalAdemMonomial;

std::string ClassicalAdemMonomial_ToString(const ClassicalAdemMonomial& aValue);

ClassicalAdemMonomial ClassicalAdemMonomial_FromString(ParsingInfo& aParsingInfo);

void ClassicalAdemMonomial_EliminateAllSq0Factors(ClassicalAdemMonomial& aMonomial);

bool ClassicalAdemMonomial_IsEqualInForm(const ClassicalAdemMonomial& aLeft, const ClassicalAdemMonomial& aRight);

bool ClassicalAdemMonomial_IsAdmissible_AssumeNoSq0Factors(const ClassicalAdemMonomial& aMonomial);

ClassicalAdemMonomial ClassicalAdemMonomial_Multiply(const ClassicalAdemMonomial& aLeft, const ClassicalAdemMonomial& aRight);
}

#endif // CORE_INCLUDE_CLASSICAL_ADEM_MONOMIAL

