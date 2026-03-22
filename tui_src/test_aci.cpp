#include <iostream>
#include <string>

#include "arbitrary_calculation_input.hpp"
#include "parsing_info.hpp"
#include "setting.hpp"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "bad number of arguments (2 expected)" << std::endl;
        return 1;
    }
    ademma_core::ParsingInfo input;
    std::string setting = argv[1];
    input.mStringToParse = argv[2];
    ademma_core::ArbitraryCalculationInput aci;
    if (setting == "cl")
    {
        aci = ademma_core::ArbitraryCalculationInput_FromString(input, ademma_core::Setting_Type::cCLASSICAL);
    }
    else if (setting == "cm")
    {
        aci = ademma_core::ArbitraryCalculationInput_FromString(input, ademma_core::Setting_Type::cC_MOTIVIC);
    }
    else if (setting == "rm")
    {
        aci = ademma_core::ArbitraryCalculationInput_FromString(input, ademma_core::Setting_Type::cR_MOTIVIC);
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
    std::cout << "parsed correctly (perhaps)" << std::endl;
    ademma_core::ACITerm aci_term_poly = ademma_core::ArbitraryCalculationInput_ExpandToPolynomial(aci);
    std::cout << ademma_core::ACITerm_ToString(aci_term_poly) << std::endl;
    ademma_core::ACITerm_Destruct(aci_term_poly);
    ademma_core::ArbitraryCalculationInput_Destruct(aci);
    return 0;
}

