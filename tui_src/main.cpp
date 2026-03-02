#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>

#include "classical_adem_math.hpp"
#include "classical_adem_polynomial.hpp"
#include "classical_adem_monomial.hpp"
#include "parsing_info.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_monomial.hpp"

// PRIVATE ENUMS (and helper functions declaration)

struct option_type_details_t
{
    const char* mName;
    const char* mDescription;
    const char* mValidValues;
};
enum option_type_specifier_e
{
    cOPTION_TYPE_SPECIFIER_HELP = 0,
    cOPTION_TYPE_SPECIFIER_SETTING,

    cOPTION_TYPE_SPECIFIER_COUNT,
    cOPTION_TYPE_SPECIFIER_NONE = cOPTION_TYPE_SPECIFIER_COUNT,
};
option_type_details_t option_type_details_from_option_type_specifier(option_type_specifier_e aValue);
option_type_specifier_e option_type_specifier_from_str(const char* aStr);
struct option_value_details_t
{
    const char* mFullName;
    const char* mShortName;
};
enum option_value_setting_e
{
    cOPTION_VALUE_SETTING_CLASSICAL = 0,
    cOPTION_VALUE_SETTING_R_MOTIVIC,

    cOPTION_VALUE_SETTING_COUNT,
    cOPTION_VALUE_SETTING_NONE = cOPTION_VALUE_SETTING_COUNT,
};
option_value_details_t option_value_details_from_option_value_setting(option_value_setting_e aValue);
option_value_setting_e option_value_setting_from_str(const char* aStr);

struct option_values_t
{
    option_value_setting_e mSetting {cOPTION_VALUE_SETTING_NONE};
};

// PRIVATE FUNCTIONS DECLARATION

enum argv_handle_return_e { cARGV_HANDLE_RETURN_SUCCESS, cARGV_HANDLE_RETURN_SUCCESS_EXIT, cARGV_HANDLE_RETURN_FAIL_EXIT };
argv_handle_return_e argv_handle(option_values_t& aOptionValues, std::string& aCalculationInput, int argc, char** argv);
bool is_token_option_specifier(const char* aToken);
void print_help();
int handle_classical();
int handle_r_motivic();

// MAIN

int main (int argc, char** argv)
{
    option_values_t option_values {};
    std::string calculation_input {};
    switch (argv_handle(option_values, calculation_input, argc, argv))
    {
        case cARGV_HANDLE_RETURN_SUCCESS:
            break;
        case cARGV_HANDLE_RETURN_SUCCESS_EXIT:
            return 0;
        case cARGV_HANDLE_RETURN_FAIL_EXIT:
            return 1;
    }
    std::cout << "Enter Steenrod Algebra type";
    std::cout << "Options:\n"
        "\tcl - Classical\n"
        "\trm - R-Motivic" << std::endl;
    std::string option;
    std::cin >> option;
    if (option == "cl")
    {
        return handle_classical();
    }
    else if (option == "rm")
    {
        return handle_r_motivic();
    }
    else
    {
        std::cout << "bad option: " << option << std::endl;
        return 1;
    }
}

// PRIVATE ENUMS (helper functions definition)

option_type_details_t option_type_details_from_option_type_specifier(option_type_specifier_e aValue)
{
    switch (aValue)
    {
        case cOPTION_TYPE_SPECIFIER_HELP:
            return {"help", "Prints this help info", ""};
        case cOPTION_TYPE_SPECIFIER_SETTING:
            return {"setting", "Which Steenrod algebra setting we are working with", "classical, cl; r-motivic, rm"};
        case cOPTION_TYPE_SPECIFIER_NONE:
            break;
    }
    return {"INVALID", "INVALID", "INVALID"};
}

option_type_specifier_e option_type_specifier_from_str(const char* aStr)
{
    option_type_specifier_e ots = cOPTION_TYPE_SPECIFIER_NONE;
    for (int ots_index = 0; ots_index < (int)cOPTION_TYPE_SPECIFIER_COUNT; ots_index++)
    {
        option_type_specifier_e ots_index_casted = (option_type_specifier_e)ots_index;
        if (std::string(aStr) == option_type_details_from_option_type_specifier(ots_index_casted).mName)
        {
            ots = ots_index_casted;
            break;
        }
    }
    return ots;
}

option_value_details_t option_value_details_from_option_value_setting(option_value_setting_e aValue)
{
    switch (aValue)
    {
        case cOPTION_VALUE_SETTING_CLASSICAL:
            return {"classical", "cl"};
        case cOPTION_VALUE_SETTING_R_MOTIVIC:
            return {"r-motivic", "rm"};
        case cOPTION_VALUE_SETTING_NONE:
            break;
    }
    return {"INVALID", "INVALID"};
}

option_value_setting_e option_value_setting_from_str(const char* aStr)
{
    option_value_setting_e ovs = cOPTION_VALUE_SETTING_NONE;
    for (int ovs_index = 0; ovs_index < (int)cOPTION_VALUE_SETTING_COUNT; ovs_index++)
    {
        option_value_setting_e ovs_index_casted = (option_value_setting_e)ovs_index;
        option_value_details_t details = option_value_details_from_option_value_setting(ovs_index_casted);
        if (std::string(aStr) == details.mFullName || std::string(aStr) == details.mShortName)
        {
            ovs = ovs_index_casted;
            break;
        }
    }
    return ovs;
}

// PRIVATE FUNCTIONS DEFINITION

argv_handle_return_e argv_handle(option_values_t& aOptionValues, std::string& aCalculationInput, int argc, char** argv)
{
    option_type_specifier_e option_type_specifier;
    bool await_option_value = false;
    char* argv_i;
    for (int i = 1; i < argc; i++)
    {
        argv_i = argv[i];
        if (await_option_value)
        {
            switch (option_type_specifier)
            {
                case cOPTION_TYPE_SPECIFIER_HELP:
                    print_help();
                    return cARGV_HANDLE_RETURN_SUCCESS_EXIT;
                case cOPTION_TYPE_SPECIFIER_SETTING:
                    aOptionValues.mSetting = option_value_setting_from_str(argv_i);
                    if (aOptionValues.mSetting == cOPTION_VALUE_SETTING_NONE)
                    {
                        option_type_details_t otd_setting = option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_SETTING);
                        std::cerr << "CLI input error: Unrecognized setting value given at index " << i << " (zero indexed): '" << argv_i << "'; valid value(s): '" << otd_setting.mValidValues << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
                        return cARGV_HANDLE_RETURN_FAIL_EXIT;
                    }
                    break;
                case cOPTION_TYPE_SPECIFIER_NONE:
                    assert(!"unreachable"); // should have been handled in the last iteration of the loop
                    std::cerr << "[INTERNAL ERROR]: Unexpectedly reached theoretically unreachable code while handling CLI arguments" << std::endl;
                    return cARGV_HANDLE_RETURN_FAIL_EXIT;
            }
            await_option_value = false;
        }
        else
        {
            if (is_token_option_specifier(argv_i))
            {
                argv_i += 2;
                option_type_specifier = option_type_specifier_from_str(argv_i);
                if (option_type_specifier == cOPTION_TYPE_SPECIFIER_NONE)
                {
                    std::cerr << "CLI input error: Unrecognized option given at index " << i << " (0 indexed): '" << argv[i] << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
                    return cARGV_HANDLE_RETURN_FAIL_EXIT;
                }
                await_option_value = true;
            }
            else
            {
                if (i + 1 != argc)
                {
                    std::cerr << "CLI input error: Non-option (options start with '--') given as not the last argument: '" << argv_i << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
                    return cARGV_HANDLE_RETURN_FAIL_EXIT;
                }
                aCalculationInput = argv_i;
            }
        }
    }
    if (await_option_value)
    {
        // argv_i must have been set to a valid value if await_option_value is true
        if (argc > 0)
        {
            std::cerr << "CLI input error: Unexpected end of arguments; value needed for option '" << argv_i << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
        }
        else
        {
            // while this case is not possible, I hate indexing into an array without knowing for certain that it's a valid index
            std::cerr << "CLI input error: Unexpected end of arguments; value needed for option '" << argv_i << "'; run with '--help' for syntax" << std::endl;
        }
        return cARGV_HANDLE_RETURN_FAIL_EXIT;
    }
    return cARGV_HANDLE_RETURN_SUCCESS;
}

bool is_token_option_specifier(const char* aToken)
{
    return (aToken[0] == '-' && aToken[1] == '-');
}

void print_help()
{
    std::cout << "TODO" << std::endl;
}

int handle_classical()
{
    using namespace ademma_core;
    std::string user_input_classical_adem_monomial {};
    std::cout << "Enter classical adem monomial (form Sq^<num1>Sq^<num2>[Sq^<num3>]...)" << std::endl;
    ParsingInfo parsing_info;
    std::cin >> parsing_info.mStringToParse;

    ClassicalAdemMonomial user_input_cam = ClassicalAdemMonomial_FromString(parsing_info);
    if (parsing_info.mErrorInfo.mIsError)
    {
        std::cout << parsing_info.GetFullErrorString() << std::endl;
        return 1;
    }
    ClassicalAdemPolynomial cap = classical_adem_math::admissify_classical_adem_monomial(user_input_cam);
    std::cout << ClassicalAdemPolynomial_ToString(cap) << std::endl;
    return 0;
}

int handle_r_motivic()
{
    using namespace ademma_core;
    std::string user_input_r_motivic_adem_monomial {};
    std::cout << "Enter R-motivic adem monomial (form eg: Sq^1\\tau\\rho^2Sq^2)" << std::endl;
    ParsingInfo parsing_info;
    std::cin >> parsing_info.mStringToParse;

    RMotivicAdemMonomial user_input_rmam = RMotivicAdemMonomial_FromString(parsing_info);
    if (parsing_info.mErrorInfo.mIsError)
    {
        std::cout << parsing_info.GetFullErrorString() << std::endl;
        return 1;
    }
    RMotivicAdemPolynomial rmap = r_motivic_adem_math::admissify_r_motivic_adem_monomial(user_input_rmam);
    std::cout << RMotivicAdemPolynomial_ToString(rmap) << std::endl;
    return 0;
}
