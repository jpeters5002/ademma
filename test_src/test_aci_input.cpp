#include "test_aci_input.hpp"

#include "arbitrary_calculation_input.hpp"
#include "c_motivic_adem_math.hpp"
#include "c_motivic_adem_monomial.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "classical_adem_math.hpp"
#include "classical_adem_monomial.hpp"
#include "classical_adem_polynomial.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_monomial.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "setting.hpp"
#include "steenrod_square.hpp"

#include <iostream>
#include <assert.h>

struct acistr_expectedstr
{
    std::string aci_string;
    std::string expected_str;
};
static const acistr_expectedstr lut_cl_cm_rm_acistr_expectedstr[] = {
    {"Sq^1Sq^3", "Sq^1Sq^3"},
};
static const acistr_expectedstr lut_cm_rm_acistr_expectedstr[] = {
    {"\\tauSq^4", "\\tauSq^4"},
};
static const acistr_expectedstr lut_rm_acistr_expectedstr[] = {
    {"\\rho\\tauSq^5", "\\rho\\tauSq^5"},
};

#define SIZEOF_LUT(lut) (sizeof(lut) / sizeof(lut[0]))

using namespace ademma_core;

bool check_expansion_correct(const std::string& aACIStr, const std::string& aExpectedStr, Setting_Type aSetting)
{
    ParsingInfo input {};
    input.mStringToParse = aACIStr;
    ArbitraryCalculationInput aci = ademma_core::ArbitraryCalculationInput_FromString(input, aSetting);
    if (input.mErrorInfo.mIsError)
    {
        std::cerr << "Error for input string \"" << aACIStr << "\" setting " << str_from_Setting_Type(aSetting) << ": " << input.GetFullErrorString() << std::endl;
        return false;
    }
    ParsingInfo expected {};
    expected.mStringToParse = aExpectedStr;
    switch (aSetting)
    {
        case Setting_Type::cCLASSICAL:
        {
            ClassicalAdemPolynomial cap_aci {};
            ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(&cap_aci, aci);
            ClassicalAdemPolynomial cap_expected = ClassicalAdemPolynomial_FromString(expected);
            return ClassicalAdemPolynomial_IsEqualInForm(cap_aci, cap_expected);
        }
        case Setting_Type::cC_MOTIVIC:
        {
            CMotivicAdemPolynomial cmap_aci {};
            ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(&cmap_aci, aci);
            CMotivicAdemPolynomial cmap_expected = CMotivicAdemPolynomial_FromString(expected);
            return CMotivicAdemPolynomial_IsEqualInForm(cmap_aci, cmap_expected);
        }
        case Setting_Type::cR_MOTIVIC:
        {
            RMotivicAdemPolynomial rmap_aci {};
            ArbitraryCalculationInput_ExpandToPolynomial_AndDestruct(&rmap_aci, aci);
            RMotivicAdemPolynomial rmap_expected = RMotivicAdemPolynomial_FromString(expected);
            return RMotivicAdemPolynomial_IsEqualInForm(rmap_aci, rmap_expected);
        }
    }
    assert(!"unreachable");
    std::cerr << "Unreachable code reached" << std::endl;
    return false;
}

int test_aci_input()
{
    bool success = true;
    for (size_t i = 0; i < SIZEOF_LUT(lut_cl_cm_rm_acistr_expectedstr); i++)
    {
        success &= check_expansion_correct(lut_cl_cm_rm_acistr_expectedstr[i].aci_string, lut_cl_cm_rm_acistr_expectedstr[i].expected_str, Setting_Type::cCLASSICAL);
        success &= check_expansion_correct(lut_cl_cm_rm_acistr_expectedstr[i].aci_string, lut_cl_cm_rm_acistr_expectedstr[i].expected_str, Setting_Type::cC_MOTIVIC);
        success &= check_expansion_correct(lut_cl_cm_rm_acistr_expectedstr[i].aci_string, lut_cl_cm_rm_acistr_expectedstr[i].expected_str, Setting_Type::cR_MOTIVIC);
    }
    for (size_t i = 0; i < SIZEOF_LUT(lut_cm_rm_acistr_expectedstr); i++)
    {
        success &= check_expansion_correct(lut_cm_rm_acistr_expectedstr[i].aci_string, lut_cm_rm_acistr_expectedstr[i].expected_str, Setting_Type::cC_MOTIVIC);
        success &= check_expansion_correct(lut_cm_rm_acistr_expectedstr[i].aci_string, lut_cm_rm_acistr_expectedstr[i].expected_str, Setting_Type::cR_MOTIVIC);
    }
    for (size_t i = 0; i < SIZEOF_LUT(lut_rm_acistr_expectedstr); i++)
    {
        success &= check_expansion_correct(lut_rm_acistr_expectedstr[i].aci_string, lut_rm_acistr_expectedstr[i].expected_str, Setting_Type::cR_MOTIVIC);
    }
    if (success)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

