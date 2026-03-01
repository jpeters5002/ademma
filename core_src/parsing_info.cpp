#include "parsing_info.hpp"

#include <stdexcept>

std::string ademma_core::ParsingInfo::GetFullErrorString()
{
    const size_t cPADDING_AROUND_ERROR_INDEX = 6;
    std::string outStr {};
    if (!mErrorInfo.mIsError)
    {
        return outStr;
    }
    outStr += "Error: ";
    outStr += mErrorInfo.mErrorString;
    if (mErrorInfo.mErrorNearbyIndex != (size_t)-1)
    {
        outStr += std::string("\n    ") + "Note: This error occurred near index " + std::to_string(mErrorInfo.mErrorNearbyIndex) + " (zero indexed)...";

        size_t error_location_string_left_index;
        if (cPADDING_AROUND_ERROR_INDEX < mErrorInfo.mErrorNearbyIndex)
        {
            error_location_string_left_index = mErrorInfo.mErrorNearbyIndex - cPADDING_AROUND_ERROR_INDEX;
        }
        else
        {
            error_location_string_left_index = 0;
        }
        size_t error_location_string_right_index;
        if (cPADDING_AROUND_ERROR_INDEX + mErrorInfo.mErrorNearbyIndex < mStringToParse.size())
        {
            error_location_string_right_index = cPADDING_AROUND_ERROR_INDEX + mErrorInfo.mErrorNearbyIndex;
        }
        else
        {
            error_location_string_right_index = mStringToParse.size() - 1;
        }

        std::string error_location_string_padded {};
        size_t error_nearby_index_in_padded_string = 0;
        if (error_location_string_left_index > 0)
        {
            error_location_string_padded += "...";
            error_nearby_index_in_padded_string = 3 + cPADDING_AROUND_ERROR_INDEX;
        }
        else
        {
            error_nearby_index_in_padded_string = mErrorInfo.mErrorNearbyIndex;
        }
        error_location_string_padded += mStringToParse.substr(error_location_string_left_index, error_location_string_right_index - error_location_string_left_index + 1);
        if (error_location_string_right_index < mStringToParse.size() - 1)
        {
            error_location_string_padded += "...";
        }

        outStr += "\n    " + error_location_string_padded;
        std::string error_location_indicator {};
        for (size_t i = 0; i < error_nearby_index_in_padded_string; i++)
        {
            error_location_indicator += ' ';
        }
        error_location_indicator += '^';
        outStr += "\n    " + error_location_indicator;
    }

    return outStr;
}

bool ademma_core::ParsingInfo::MatchString(const std::string& aString)
{
    for (size_t i = 0; i < aString.size(); i++)
    {
        if (mCurrentIndex + i >= mStringToParse.size())
        {
            return false;
        }
        char user_char = mStringToParse[mCurrentIndex + i];
        if (user_char != aString[i])
        {
            return false;
        }
    }
    return true;
}

bool ademma_core::ParsingInfo::MatchString_IncreaseIndexOnSuccess(const std::string& aString)
{
    if (MatchString(aString))
    {
        mCurrentIndex += aString.size();
        return true;
    }
    return false;
}

bool ademma_core::ParsingInfo::ParseInt(int& aIntOut)
{
    try
    {
        aIntOut = std::stoi(mStringToParse.substr(mCurrentIndex));
    }
    catch (std::invalid_argument&)
    {
        return false;
    }
    catch (std::out_of_range&)
    {
        return false;
    }
    return true;
}

void ademma_core::ParsingInfo::IncreaseIndexOverInt()
{
    bool first = true;
    for (;;)
    {
        if (mCurrentIndex >= mStringToParse.size())
        {
            break;
        }
        char c = mStringToParse[mCurrentIndex];
        if ((first && c == '-') || (c >= '0' && c <= '9'))
        {
            first = false;
            mCurrentIndex++;
        }
        else
        {
            break;
        }
    }
}

