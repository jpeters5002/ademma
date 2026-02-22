#include "parsing_info.hpp"

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

