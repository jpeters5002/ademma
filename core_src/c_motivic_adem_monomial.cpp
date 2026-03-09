#include "c_motivic_adem_monomial.hpp"

#include <stdexcept>

#include <assert.h>

#include "steenrod_square.hpp"

// FACTOR

ademma_core::CMotivicAdemMonomialFactor_Type ademma_core::CMotivicAdemMonomialFactor_GetType(CMotivicAdemMonomialFactor aValue)
{
    if (aValue & cCMotivicAdemMonomialFactor_IS_TAU_BIT)
    {
        return CMotivicAdemMonomialFactor_Type::cTau;
    }
    else
    {
        return CMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree;
    }
}

ademma_core::CMotivicAdemMonomialFactor ademma_core::CMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(SteenrodSquareDegree aValue)
{
    if (aValue & cCMotivicAdemMonomialFactor_IS_TAU_BIT)
    {
        throw std::runtime_error("too large steenrod square degree (would clobber allocated bits)");
    }
    return (CMotivicAdemMonomialFactor)aValue;
}

ademma_core::CMotivicAdemMonomialFactor ademma_core::CMotivicAdemMonomialFactor_CreateTau(int aPower)
{
    if (aPower & (cCMotivicAdemMonomialFactor_IS_TAU_BIT))
    {
        throw std::runtime_error("too many tau factors (would clobber allocated bits if combined)");
    }
    return (cCMotivicAdemMonomialFactor_IS_TAU_BIT | aPower);
}

std::string ademma_core::CMotivicAdemMonomialFactor_ToString(CMotivicAdemMonomialFactor aValue)
{
    std::string strOut {};
    CMotivicAdemMonomialFactor_Type type = CMotivicAdemMonomialFactor_GetType(aValue);
    switch (type)
    {
        case CMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree:
            return SteenrodSquareDegree_ToString((SteenrodSquareDegree)aValue);
        case CMotivicAdemMonomialFactor_Type::cTau:
            strOut = "{\\tau}";
            break;
        default:
            throw std::runtime_error("unreachable code reached: CMotivicAdemMonomialFactor_GetType unexpectedly returned cNONE or bad value");
    }
    int power = CMotivicAdemMonomialFactor_GetPower_AssumeTau(aValue);
    if (power != 1)
    {
        strOut += "^{";
        strOut += std::to_string(power);
        strOut += "}";
    }
    return strOut;
}

ademma_core::CMotivicAdemMonomialFactor ademma_core::CMotivicAdemMonomialFactor_FromString(ParsingInfo& aParsingInfo)
{
    CMotivicAdemMonomialFactor cmamfOut;
    if (aParsingInfo.MatchString("Sq^"))
    {
        return (CMotivicAdemMonomialFactor)SteenrodSquareDegree_FromString(aParsingInfo);
    }
    else if (aParsingInfo.MatchString_IncreaseIndexOnSuccess("\\tau"))
    {}
    else
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorString = "Unrecognized character combination while parsing C-Motivic adem monomial factor; expected form: 'Sq^<num>' OR '\\tau' OR '\\tau^<num>' (optional curly brackets around <num>)";
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        return cCMotivicAdemMonomialFactor_ERROR_VALUE;
    }

    int power = 1;
    if (aParsingInfo.MatchString_IncreaseIndexOnSuccess("^"))
    {
        bool is_bracketed_power = false;
        if (aParsingInfo.MatchString_IncreaseIndexOnSuccess("{"))
        {
            is_bracketed_power = true;
        }
        if (!aParsingInfo.ParseInt(power))
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Failed to parse int while parsing C-Motivic adem monomial factor";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return cCMotivicAdemMonomialFactor_ERROR_VALUE;
        }
        if (power < 0)
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Negative value given to power of tau, which is meaningless";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return cCMotivicAdemMonomialFactor_ERROR_VALUE;
        }
        if (power & (cCMotivicAdemMonomialFactor_IS_TAU_BIT))
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Too large value for power of tau would clobber allocated info bits (" + std::to_string(sizeof(CMotivicAdemMonomialFactor) * 8 - 2) + " bits available for the power)";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return cCMotivicAdemMonomialFactor_ERROR_VALUE;
        }
        aParsingInfo.IncreaseIndexOverInt();
        if (is_bracketed_power && !aParsingInfo.MatchString_IncreaseIndexOnSuccess("}"))
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "When parsing power " + std::to_string(power) + ", a start bracket '{' was found, but no end bracket '}'";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return cCMotivicAdemMonomialFactor_ERROR_VALUE;
        }
    }
    cmamfOut = CMotivicAdemMonomialFactor_CreateTau(power);
    return cmamfOut;
}

int ademma_core::CMotivicAdemMonomialFactor_GetPower_AssumeTau(CMotivicAdemMonomialFactor aValue)
{
    return aValue & ~(cCMotivicAdemMonomialFactor_IS_TAU_BIT);
}

bool ademma_core::CMotivicAdemMonomialFactor_IsPairAdmissible(CMotivicAdemMonomialFactor aLeft, CMotivicAdemMonomialFactor aRight)
{
    CMotivicAdemMonomialFactor_Type type_left = CMotivicAdemMonomialFactor_GetType(aLeft);
    CMotivicAdemMonomialFactor_Type type_right = CMotivicAdemMonomialFactor_GetType(aRight);
    if (type_right == CMotivicAdemMonomialFactor_Type::cTau)
    {
        return false;
    }
    // r = steenrod_square_degree
    if (type_left == CMotivicAdemMonomialFactor_Type::cTau)
    {
        return true;
    }
    // l = steenrod_square_degree, r = steenrod_square_degree
    assert(type_left == CMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree && type_left == type_right);
    return SteenrodSquareDegree_IsPairAdmissible((SteenrodSquareDegree)aLeft, (SteenrodSquareDegree)aRight);
}

// MONOMIAL

std::string ademma_core::CMotivicAdemMonomial_ToString(const CMotivicAdemMonomial& aValue)
{
    std::string outStr = "";
    bool previous_factor_tau = false;
    for (size_t i = 0; i < aValue.size(); i++)
    {
        if (previous_factor_tau)
        {
            outStr += ' ';
        }
        outStr += CMotivicAdemMonomialFactor_ToString(aValue[i]);
        switch (CMotivicAdemMonomialFactor_GetType(aValue[i]))
        {
            case CMotivicAdemMonomialFactor_Type::cTau:
                previous_factor_tau = true;
                break;
            default:
                previous_factor_tau = false;
        }
    }
    return outStr;
}

ademma_core::CMotivicAdemMonomial ademma_core::CMotivicAdemMonomial_FromString(ParsingInfo& aParsingInfo)
{
    CMotivicAdemMonomial cmamOut {};
    for (;;)
    {
        aParsingInfo.IncreaseIndexOverWhitespace();
        if (aParsingInfo.mCurrentIndex >= aParsingInfo.mStringToParse.size())
        {
            break;
        }
        SteenrodSquareDegree cmamf = CMotivicAdemMonomialFactor_FromString(aParsingInfo);
        if (aParsingInfo.mErrorInfo.mIsError)
        {
            return {};
        }
        cmamOut.push_back(cmamf);
    }
    return cmamOut;
}

void ademma_core::CMotivicAdemMonomial_EliminateAllSq0Factors(CMotivicAdemMonomial& aMonomial)
{
    for (size_t i = aMonomial.size(); i > 0; i--)
    {
        if (aMonomial[i - 1] == 0)
        {
            aMonomial.erase(aMonomial.begin() + (i - 1));
        }
    }
}

void ademma_core::CMotivicAdemMonomial_ShoveTauLeft(CMotivicAdemMonomial& aMonomial)
{
    int power_count = 0;
    for (size_t i = aMonomial.size(); i > 0; i--)
    {
        CMotivicAdemMonomialFactor cmamf = aMonomial[i - 1];
        CMotivicAdemMonomialFactor_Type type = CMotivicAdemMonomialFactor_GetType(cmamf);
        if (type != CMotivicAdemMonomialFactor_Type::cTau)
        {
            continue;
        }
        if (i - 1 == 0 && power_count == 0)
        {
            return;
        }
        power_count += CMotivicAdemMonomialFactor_GetPower_AssumeTau(cmamf);
        aMonomial.erase(aMonomial.begin() + (i - 1));
    }
    if (power_count > 0)
    {
        CMotivicAdemMonomialFactor combined_taus = CMotivicAdemMonomialFactor_CreateTau(power_count);
        aMonomial.insert(aMonomial.begin(), combined_taus);
    }
}

bool ademma_core::CMotivicAdemMonomial_IsEqualInForm(const CMotivicAdemMonomial& aLeft, const CMotivicAdemMonomial& aRight)
{
    if (aLeft.size() != aRight.size())
    {
        return false;
    }
    for (size_t i = 0; i < aLeft.size(); i++)
    {
        if (aLeft[i] != aRight[i])
        {
            return false;
        }
    }
    return true;
}

bool ademma_core::CMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(const CMotivicAdemMonomial& aMonomial)
{
    if (aMonomial.size() < 2)
    {
        return true;
    }
    CMotivicAdemMonomialFactor previous_factor = aMonomial[0];
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        if (!CMotivicAdemMonomialFactor_IsPairAdmissible(previous_factor, aMonomial[i]))
        {
            return false;
        }
        previous_factor = aMonomial[i];
    }
    return true;
}

ademma_core::CMotivicAdemMonomial ademma_core::CMotivicAdemMonomial_Multiply(const CMotivicAdemMonomial& aLeft, const CMotivicAdemMonomial& aRight)
{
    CMotivicAdemMonomial cmamOut = aLeft;
    cmamOut.insert(std::end(cmamOut), std::begin(aRight), std::end(aRight));
    return cmamOut;
}

