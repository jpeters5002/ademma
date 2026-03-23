#include <iostream>
#include <string>

#include "arbitrary_calculation_input.hpp"
#include "parsing_info.hpp"
#include "setting.hpp"

int main(int argc, char** argv)
{
    using namespace ademma_core;
    if (argc != 3)
    {
        std::cout << "bad number of arguments (2 expected)" << std::endl;
        return 1;
    }
    ParsingInfo input;
    std::string setting = argv[1];
    input.mStringToParse = argv[2];
    ArbitraryCalculationInput aci;
    if (setting == "cl")
    {
        aci = ArbitraryCalculationInput_FromString(input, Setting_Type::cCLASSICAL);
        CMotivicAdemPolynomial cap {};
        ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(&cap, aci);
        std::cout << ClassicalAdemPolynomial_ToString(cap) << std::endl;
    }
    else if (setting == "cm")
    {
        aci = ArbitraryCalculationInput_FromString(input, Setting_Type::cC_MOTIVIC);
        CMotivicAdemPolynomial cmap {};
        ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(&cmap, aci);
        std::cout << CMotivicAdemPolynomial_ToString(cmap) << std::endl;
    }
    else if (setting == "rm")
    {
        aci = ArbitraryCalculationInput_FromString(input, Setting_Type::cR_MOTIVIC);
        RMotivicAdemPolynomial rmap {};
        ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(&rmap, aci);
        std::cout << RMotivicAdemPolynomial_ToString(rmap) << std::endl;
    }
    else
    {
        std::cout << "bad setting; expected cl, cm, rm; found: " << setting << std::endl;
        return 1;
    }
    if (input.mErrorInfo.mIsError)
    {
        std::cout << input.GetFullErrorString() << std::endl;
        return 1;
    }
    return 0;
}

