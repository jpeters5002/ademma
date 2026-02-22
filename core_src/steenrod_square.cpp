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
    return "Sq^" + std::to_string(aValue);
}

ademma_core::SteenrodSquareDegree ademma_core::SteenrodSquareDegree_FromString(ParsingInfo& aParsingInfo)
{
    SteenrodSquareDegree degreeOut;
    size_t chars_handled_here = 0;
    const char* expected_str = "Sq^";
    for (size_t i = 0; i < 3; i++)
    {
        assert(i < strlen(expected_str));
        if (aParsingInfo.mCurrentIndex >= aParsingInfo.mStringToParse.size())
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Unexpected end of data while parsing a steenrod square; expected form: Sq^<num>";
            return cSteenrodSquareDegree_ERROR_VALUE;
        }
        char user_char = aParsingInfo.mStringToParse[aParsingInfo.mCurrentIndex];
        if (user_char != expected_str[i])
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = std::string("Unexpected character while parsing steenrod square: ") + user_char + "; expected form: Sq^<num>";
            return cSteenrodSquareDegree_ERROR_VALUE;
        }
        aParsingInfo.mCurrentIndex++;
    }
    try
    {
        degreeOut = std::stoi(aParsingInfo.mStringToParse, &aParsingInfo.mCurrentIndex);
    }
    catch (std::invalid_argument& aException)
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        aParsingInfo.mErrorInfo.mErrorString = "Unable to determine steenrod square degree value; expected form: Sq^<num>";
        return cSteenrodSquareDegree_ERROR_VALUE;
    }
    catch (std::out_of_range& aException)
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        aParsingInfo.mErrorInfo.mErrorString = "Unable to determine steenrod square degree value: value out of range of int";
        return cSteenrodSquareDegree_ERROR_VALUE;
    }
    if (degreeOut < 0)
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        aParsingInfo.mErrorInfo.mErrorString = "Given steenrod square degree value is negative, which is meaningless: " + std::to_string(degreeOut);
        return cSteenrodSquareDegree_ERROR_VALUE;
    }
    for (;;)
    {
        if (aParsingInfo.mCurrentIndex >= aParsingInfo.mStringToParse.size())
        {
            break;
        }
        char c = aParsingInfo.mStringToParse[aParsingInfo.mCurrentIndex];
        if (c >= '0' && c <= '9')
        {
            aParsingInfo.mCurrentIndex++;
        }
        else
        {
            break;
        }
    }
    return degreeOut;
}

bool ademma_core::SteenrodSquareDegree_IsPairAdmissible(SteenrodSquareDegree aLeft, SteenrodSquareDegree aRight)
{
    return aLeft >= 2 * aRight;
}

