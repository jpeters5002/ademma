#include "steenrod_square.hpp"

#include <string>
#include <stdexcept>

#include <assert.h>
#include <string.h>

std::string ademma_core::SteenrodSquareDegree_ToString(SteenrodSquareDegree aValue)
{
    if (aValue == cSteenrodSquareDegree_ERROR_VALUE)
    {
        throw std::runtime_error("error value SteenrodSquareDegree given to SteenrodSquareDegree_ToString");
    }
    return "Sq^{" + std::to_string(aValue) + "}";
}

ademma_core::SteenrodSquareDegree ademma_core::SteenrodSquareDegree_FromString(ParsingInfo& aParsingInfo)
{
    SteenrodSquareDegree degreeOut;
    if (!aParsingInfo.MatchString_IncreaseIndexOnSuccess("Sq^"))
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        aParsingInfo.mErrorInfo.mErrorString = "Unrecognized character combination while parsing Steenrod square degree; expected form: Sq^<num> (optional curly brackets around <num>)";
        return cSteenrodSquareDegree_ERROR_VALUE;
    }
    bool is_bracketed_superscript = false;
    if (aParsingInfo.MatchString_IncreaseIndexOnSuccess("{"))
    {
        is_bracketed_superscript = true;
    }
    if (!aParsingInfo.ParseInt(degreeOut))
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        aParsingInfo.mErrorInfo.mErrorString = "Failed to parse int while parsing Steenrod square degree";
        return cSteenrodSquareDegree_ERROR_VALUE;
    }
    if (degreeOut < 0)
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        aParsingInfo.mErrorInfo.mErrorString = "Given Steenrod square degree value is negative, which is meaningless: " + std::to_string(degreeOut);
        return cSteenrodSquareDegree_ERROR_VALUE;
    }
    aParsingInfo.IncreaseIndexOverInt();
    if (is_bracketed_superscript && !aParsingInfo.MatchString_IncreaseIndexOnSuccess("}"))
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorString = "When parsing Steenrod square degree " + std::to_string(degreeOut) + ", a start bracket '{' was found, but no end bracket '}'";
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        return cSteenrodSquareDegree_ERROR_VALUE;
    }
    return degreeOut;
}

bool ademma_core::SteenrodSquareDegree_IsPairAdmissible(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight)
{
    return aLeft >= 2 * aRight;
}

