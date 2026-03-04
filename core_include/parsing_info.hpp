#ifndef CORE_INCLUDE_PARSING_INFO_HPP
#define CORE_INCLUDE_PARSING_INFO_HPP

#include <string>

namespace ademma_core
{
struct ParsingErrorInfo
{
    bool mIsError {false};
    std::string mErrorString {};
    size_t mErrorNearbyIndex {(size_t)-1};
};
struct ParsingInfo
{
    std::string mStringToParse {};
    size_t mCurrentIndex {0};

    ParsingErrorInfo mErrorInfo {};

    std::string GetFullErrorString();
    bool MatchString(const std::string& aString) const;
    bool MatchString_IncreaseIndexOnSuccess(const std::string& aString);
    bool ParseInt(int& aIntOut);
    void IncreaseIndexOverInt();
    void IncreaseIndexOverWhitespace();
};
}

#endif // CORE_INCLUDE_PARSING_INFO_HPP

