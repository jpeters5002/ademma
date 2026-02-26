#include "r_motivic_adem_monomial.hpp"

#include <stdexcept>

#include <assert.h>

#include "steenrod_square.hpp"

// FACTOR

ademma_core::RMotivicAdemMonomialFactor_Type ademma_core::RMotivicAdemMonomialFactor_GetType(RMotivicAdemMonomialFactor aValue)
{
    if (aValue & cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT)
    {
        if (aValue & cRMotivicAdemMonomialFactor_IS_TAU_BIT)
        {
            return RMotivicAdemMonomialFactor_Type::cTau;
        }
        else
        {
            return RMotivicAdemMonomialFactor_Type::cRho;
        }
    }
    else
    {
        return RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree;
    }
}

ademma_core::RMotivicAdemMonomialFactor ademma_core::RMotivicAdemMonomialFactor_CreateSteenrodSquareDegree(SteenrodSquareDegree aValue)
{
    if (aValue & cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT)
    {
        throw std::runtime_error("too large steenrod square degree (would clobber allocated bits)");
    }
    return (RMotivicAdemMonomialFactor)aValue;
}

ademma_core::RMotivicAdemMonomialFactor ademma_core::RMotivicAdemMonomialFactor_CreateTau(int aPower)
{
    if (aPower & (cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT | cRMotivicAdemMonomialFactor_IS_TAU_BIT))
    {
        throw std::runtime_error("too many tau factors (would clobber allocated bits if combined)");
    }
    return (cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT | cRMotivicAdemMonomialFactor_IS_TAU_BIT | aPower);
}

ademma_core::RMotivicAdemMonomialFactor ademma_core::RMotivicAdemMonomialFactor_CreateRho(int aPower)
{
    if (aPower & (cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT | cRMotivicAdemMonomialFactor_IS_TAU_BIT))
    {
        throw std::runtime_error("too many rho factors (would clobber allocated bits if combined)");
    }
    return (cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT | aPower);
}

std::string ademma_core::RMotivicAdemMonomialFactor_ToString(RMotivicAdemMonomialFactor aValue)
{
    std::string strOut {};
    RMotivicAdemMonomialFactor_Type type = RMotivicAdemMonomialFactor_GetType(aValue);
    switch (type)
    {
        case RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree:
            return SteenrodSquareDegree_ToString((SteenrodSquareDegree)aValue);
        case RMotivicAdemMonomialFactor_Type::cTau:
            strOut = "\\tau";
            break;
        case RMotivicAdemMonomialFactor_Type::cRho:
            strOut = "\\rho";
            break;
        default:
            throw std::runtime_error("unreachable code reached: RMotivicAdemMonomialFactor_GetType unexpectedly returned cNONE or bad value");
    }
    int power = RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(aValue);
    if (power == 1)
    {
        return strOut;
    }
    strOut += '^';
    strOut += std::to_string(power);
    return strOut;
}

ademma_core::RMotivicAdemMonomialFactor ademma_core::RMotivicAdemMonomialFactor_FromString(ParsingInfo& aParsingInfo)
{
    RMotivicAdemMonomialFactor rmamfOut;
    bool is_tau;
    if (aParsingInfo.MatchString("Sq^"))
    {
        return (RMotivicAdemMonomialFactor)SteenrodSquareDegree_FromString(aParsingInfo);
    }
    else if (aParsingInfo.MatchString_IncreaseIndexOnSuccess("\\tau"))
    {
        is_tau = true;
    }
    else if (aParsingInfo.MatchString_IncreaseIndexOnSuccess("\\rho"))
    {
        is_tau = false;
    }
    else
    {
        aParsingInfo.mErrorInfo.mIsError = true;
        aParsingInfo.mErrorInfo.mErrorString = "Unrecognized character combination while parsing R-motivic adem monomial factor; expected form: 'Sq^<num>' OR '\\tau' OR '\\tau^<num>' OR '\\rho' OR '\\rho^<num>'";
        aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
        return cRMotivicAdemMonomialFactor_ERROR_VALUE;
    }

    int power = 1;
    if (aParsingInfo.MatchString_IncreaseIndexOnSuccess("^"))
    {
        if (!aParsingInfo.ParseInt(power))
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Failed to parse int while parsing R-motivic adem monomial factor";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return cRMotivicAdemMonomialFactor_ERROR_VALUE;
        }
        if (power < 0)
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Negative value given to power of rho or tau, which is meaningless";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return cRMotivicAdemMonomialFactor_ERROR_VALUE;
        }
        if (power & (cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT | cRMotivicAdemMonomialFactor_IS_TAU_BIT))
        {
            aParsingInfo.mErrorInfo.mIsError = true;
            aParsingInfo.mErrorInfo.mErrorString = "Too large value for power of rho or tau would clobber allocated info bits (", std::to_string(sizeof(RMotivicAdemMonomialFactor) * 8 - 2), " bits available for the power)";
            aParsingInfo.mErrorInfo.mErrorNearbyIndex = aParsingInfo.mCurrentIndex;
            return cRMotivicAdemMonomialFactor_ERROR_VALUE;
        }
        aParsingInfo.IncreaseIndexOverInt();
    }
    if (is_tau)
    {
        rmamfOut = RMotivicAdemMonomialFactor_CreateTau(power);
    }
    else
    {
        rmamfOut = RMotivicAdemMonomialFactor_CreateRho(power);
    }
    return rmamfOut;
}

int ademma_core::RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(RMotivicAdemMonomialFactor aValue)
{
    return aValue & ~(cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT | cRMotivicAdemMonomialFactor_IS_TAU_BIT);
}

bool ademma_core::RMotivicAdemMonomialFactor_IsPairAdmissible(RMotivicAdemMonomialFactor aLeft, RMotivicAdemMonomialFactor aRight)
{
    RMotivicAdemMonomialFactor_Type type_left = RMotivicAdemMonomialFactor_GetType(aLeft);
    RMotivicAdemMonomialFactor_Type type_right = RMotivicAdemMonomialFactor_GetType(aRight);
    if (type_left == RMotivicAdemMonomialFactor_Type::cRho)
    {
        return type_right != type_left;
    }
    // l != rho
    if (type_right != RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree)
    {
        return false;
    }
    // l != rho, r = steenrod_square_degree
    if (type_left == RMotivicAdemMonomialFactor_Type::cTau)
    {
        return true;
    }
    // l = steenrod_square_degree, r = steenrod_square_degree
    assert(type_left == RMotivicAdemMonomialFactor_Type::cSteenrodSquareDegree && type_left == type_right);
    return SteenrodSquareDegree_IsPairAdmissible((SteenrodSquareDegree)aLeft, (SteenrodSquareDegree)aRight);
}

// MONOMIAL

std::string ademma_core::RMotivicAdemMonomial_ToString(const RMotivicAdemMonomial& aValue)
{
    std::string outStr = "";
    for (size_t i = 0; i < aValue.size(); i++)
    {
        outStr += RMotivicAdemMonomialFactor_ToString(aValue[i]);
    }
    return outStr;
}

ademma_core::RMotivicAdemMonomial ademma_core::RMotivicAdemMonomial_FromString(ParsingInfo& aParsingInfo)
{
    RMotivicAdemMonomial rmamOut {};
    for (;;)
    {
        if (aParsingInfo.mCurrentIndex >= aParsingInfo.mStringToParse.size())
        {
            break;
        }
        SteenrodSquareDegree rmamf = RMotivicAdemMonomialFactor_FromString(aParsingInfo);
        if (aParsingInfo.mErrorInfo.mIsError)
        {
            return {};
        }
        rmamOut.push_back(rmamf);
    }
    return rmamOut;
}

void ademma_core::RMotivicAdemMonomial_EliminateAllPower0Taus(RMotivicAdemMonomial& aMonomial)
{
    for (size_t i = aMonomial.size(); i > 0; i--)
    {
        if (aMonomial[i - 1] == cRMotivicAdemMonomialFactor_IS_RHO_OR_TAU_BIT & cRMotivicAdemMonomialFactor_IS_TAU_BIT)
        {
            aMonomial.erase(aMonomial.begin() + (i - 1));
        }
    }
}

void ademma_core::RMotivicAdemMonomial_EliminateAllSq0Factors(RMotivicAdemMonomial& aMonomial)
{
    for (size_t i = aMonomial.size(); i > 0; i--)
    {
        if (aMonomial[i - 1] == 0)
        {
            aMonomial.erase(aMonomial.begin() + (i - 1));
        }
    }
}

void ademma_core::RMotivicAdemMonomial_ShoveRhoLeft(RMotivicAdemMonomial& aMonomial)
{
    int power_count = 0;
    for (size_t i = aMonomial.size(); i > 0; i--)
    {
        RMotivicAdemMonomialFactor rmamf = aMonomial[i - 1];
        RMotivicAdemMonomialFactor_Type type = RMotivicAdemMonomialFactor_GetType(rmamf);
        if (type != RMotivicAdemMonomialFactor_Type::cRho)
        {
            continue;
        }
        if (i - 1 == 0 && power_count == 0)
        {
            return;
        }
        power_count += RMotivicAdemMonomialFactor_GetPower_AssumeRhoOrTau(rmamf);
        aMonomial.erase(aMonomial.begin() + (i - 1));
    }
    if (power_count > 0)
    {
        RMotivicAdemMonomialFactor combined_rhos = RMotivicAdemMonomialFactor_CreateRho(power_count);
        aMonomial.insert(aMonomial.begin(), combined_rhos);
    }
}

bool ademma_core::RMotivicAdemMonomial_IsEqualInForm(const RMotivicAdemMonomial& aLeft, const RMotivicAdemMonomial& aRight)
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

bool ademma_core::RMotivicAdemMonomial_IsAdmissible_AssumeNoSq0Factors(const RMotivicAdemMonomial& aMonomial)
{
    if (aMonomial.size() < 2)
    {
        return true;
    }
    RMotivicAdemMonomialFactor previous_factor = aMonomial[0];
    for (size_t i = 1; i < aMonomial.size(); i++)
    {
        if (!RMotivicAdemMonomialFactor_IsPairAdmissible(previous_factor, aMonomial[i]))
        {
            return false;
        }
        previous_factor = aMonomial[i];
    }
    return true;
}

ademma_core::RMotivicAdemMonomial ademma_core::RMotivicAdemMonomial_Multiply(const RMotivicAdemMonomial& aLeft, const RMotivicAdemMonomial& aRight)
{
    RMotivicAdemMonomial rmamOut = aLeft;
    rmamOut.insert(std::end(rmamOut), std::begin(aRight), std::end(aRight));
    return rmamOut;
}

