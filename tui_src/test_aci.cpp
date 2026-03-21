#include <iostream>
#include <string>

#include "arbitrary_calculation_input.hpp"
#include "parsing_info.hpp"
#include "setting.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "bad number of arguments (1 expected)" << std::endl;
        return 1;
    }
    ademma_core::ParsingInfo input;
    input.mStringToParse = argv[1];
    ademma_core::ArbitraryCalculationInput aci = ademma_core::ArbitraryCalculationInput_FromString(input, ademma_core::Setting_Type::cCLASSICAL);
    if (input.mErrorInfo.mIsError)
    {
        std::cout << input.GetFullErrorString() << std::endl;
        return 1;
    }
    std::cout << "parsed correctly (perhaps)" << std::endl;
    std::cout << ademma_core::ArbitraryCalculationInput_ToString(aci) << std::endl;
    ademma_core::ArbitraryCalculationInput_CoagulateInnermostToPoly_Recursive(aci);
    std::cout << ademma_core::ArbitraryCalculationInput_ToString(aci) << std::endl;
    ademma_core::ArbitraryCalculationInput_ExpandPolyExponent_Recursive(aci);
    std::cout << ademma_core::ArbitraryCalculationInput_ToString(aci) << std::endl;
    ademma_core::ArbitraryCalculationInput_ExpandPolyFoil_Recursive(aci);
    std::cout << ademma_core::ArbitraryCalculationInput_ToString(aci) << std::endl;
    ademma_core::ArbitraryCalculationInput_Destruct(aci);
    return 0;
}

