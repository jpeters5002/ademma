#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>

#include "c_motivic_adem_math.hpp"
#include "c_motivic_adem_polynomial.hpp"
#include "c_motivic_adem_monomial.hpp"
#include "classical_adem_math.hpp"
#include "classical_adem_polynomial.hpp"
#include "classical_adem_monomial.hpp"
#include "cmake_generated_version_info.hpp"
#include "parsing_info.hpp"
#include "r_motivic_adem_math.hpp"
#include "r_motivic_adem_polynomial.hpp"
#include "r_motivic_adem_monomial.hpp"

// PRIVATE ENUMS (and helper functions declaration)

struct option_type_details_t
{
    const char* mName;
    const char* mDescription;
    bool mExpectValue;
    const char* mValidValues;
};
enum option_type_specifier_e
{
    cOPTION_TYPE_SPECIFIER_VERSION = 0,
    cOPTION_TYPE_SPECIFIER_HELP,
    cOPTION_TYPE_SPECIFIER_ONLY_CLI,
    cOPTION_TYPE_SPECIFIER_NO_LOOP_TUI,
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
    cOPTION_VALUE_SETTING_C_MOTIVIC,

    cOPTION_VALUE_SETTING_COUNT,
    cOPTION_VALUE_SETTING_NONE = cOPTION_VALUE_SETTING_COUNT,
};
option_value_details_t option_value_details_from_option_value_setting(option_value_setting_e aValue);
option_value_setting_e option_value_setting_from_str(const char* aStr);

struct option_values_t
{
    bool mOnlyCLI {false};
    bool mNoLoopTUI {false};
    option_value_setting_e mSetting {cOPTION_VALUE_SETTING_NONE};
};

// PRIVATE FUNCTIONS DECLARATION

enum control_return_e { cCONTROL_RETURN_SUCCESS, cCONTROL_RETURN_SUCCESS_EXIT, cCONTROL_RETURN_FAIL };
control_return_e argv_handle(option_values_t& aOptionValues, std::string& aCalculationInput, int argc, char** argv);
bool is_token_option_specifier(const char* aToken);
void print_help();
control_return_e handle_classical(const option_values_t& aOptionValues, const std::string& aCalculationInput);
control_return_e handle_r_motivic(const option_values_t& aOptionValues, const std::string& aCalculationInput);
control_return_e handle_c_motivic(const option_values_t& aOptionValues, const std::string& aCalculationInput);

// MAIN

int main (int argc, char** argv)
{
    option_values_t option_values_argv_chosen {};
    option_values_t option_values_using {};
    std::string calculation_input_argv_chosen {};
    std::string calculation_input_using {};
    switch (argv_handle(option_values_argv_chosen, calculation_input_argv_chosen, argc, argv))
    {
        case cCONTROL_RETURN_SUCCESS:
            break;
        case cCONTROL_RETURN_SUCCESS_EXIT:
            return 0;
        case cCONTROL_RETURN_FAIL:
            return 1;
    }
    for (size_t i = 0; i < calculation_input_argv_chosen.size(); i++)
    {
        if (calculation_input_argv_chosen[i] == 'B')
        {
            calculation_input_argv_chosen[i] = '\\';
        }
    }
    option_values_using = option_values_argv_chosen;
    calculation_input_using = calculation_input_argv_chosen;
    for (;;)
    {
        if (option_values_using.mSetting == cOPTION_VALUE_SETTING_NONE)
        {
            if (option_values_using.mOnlyCLI)
            {
                std::cerr << "CLI input error: Option '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_ONLY_CLI).mName << "' was given but specifying '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_SETTING).mName << "' is required (and was not given)" << std::endl;
                return 1;
            }
            else
            {
                for (;;)
                {
                    std::cout << "Enter Setting" << std::endl;
                    std::cout << "Options:\n"
                        "\tq  - Quit Program\n"
                        "\tcl - Classical\n"
                        "\trm - R-Motivic\n"
                        "\tcm - C-Motivic" << std::endl;
                    std::string option;
                    std::getline(std::cin, option);
                    if (option == "cl")
                    {
                        option_values_using.mSetting = cOPTION_VALUE_SETTING_CLASSICAL;
                        break;
                    }
                    else if (option == "rm")
                    {
                        option_values_using.mSetting = cOPTION_VALUE_SETTING_R_MOTIVIC;
                        break;
                    }
                    else if (option == "cm")
                    {
                        option_values_using.mSetting = cOPTION_VALUE_SETTING_C_MOTIVIC;
                        break;
                    }
                    else if (option == "q")
                    {
                        return 0;
                    }
                    else
                    {
                        std::cerr << "bad setting: " << option << std::endl;
                    }
                }
            }
        }
        control_return_e subhandle_control_return;
        switch (option_values_using.mSetting)
        {
            case cOPTION_VALUE_SETTING_CLASSICAL:
                subhandle_control_return = handle_classical(option_values_using, calculation_input_using);
                break;
            case cOPTION_VALUE_SETTING_R_MOTIVIC:
                subhandle_control_return = handle_r_motivic(option_values_using, calculation_input_using);
                break;
            case cOPTION_VALUE_SETTING_C_MOTIVIC:
                subhandle_control_return = handle_c_motivic(option_values_using, calculation_input_using);
                break;
            case cOPTION_VALUE_SETTING_NONE:
                goto error_unreachable_code_setting_type;
        }
        switch (subhandle_control_return)
        {
            case cCONTROL_RETURN_SUCCESS:
                break;
            case cCONTROL_RETURN_SUCCESS_EXIT:
                return 0;
            case cCONTROL_RETURN_FAIL:
                break;
        }
        if (option_values_using.mOnlyCLI || option_values_using.mNoLoopTUI || (option_values_argv_chosen.mSetting != cOPTION_VALUE_SETTING_NONE && !calculation_input_argv_chosen.empty()))
        {
            return (subhandle_control_return == cCONTROL_RETURN_FAIL) ? 1 : 0;
        }
        option_values_using = option_values_argv_chosen;
        calculation_input_using = "";
        std::cout << std::endl;
    }
error_unreachable_code_setting_type:
    std::cerr << "[INTERNAL ERROR]: Unexpectedly reached theoretically unreachable code regarding setting type" << std::endl;
    return 1;
}

// PRIVATE ENUMS (helper functions definition)

option_type_details_t option_type_details_from_option_type_specifier(option_type_specifier_e aValue)
{
    switch (aValue)
    {
        case cOPTION_TYPE_SPECIFIER_VERSION:
            return {"version", "Print the version number and exit", false, ""};
        case cOPTION_TYPE_SPECIFIER_HELP:
            return {"help", "Print this help info and exit", false, ""};
        case cOPTION_TYPE_SPECIFIER_ONLY_CLI:
            return {"only-cli", "Expect all input to be specified through CLI arugments (makes option 'setting' and argument 'calculation-input' required)", false, ""};
        case cOPTION_TYPE_SPECIFIER_NO_LOOP_TUI:
            return {"no-loop-tui", "Exit after first calculation when using the TUI instead of looping", false, ""};
        case cOPTION_TYPE_SPECIFIER_SETTING:
            return {"setting", "Which Steenrod algebra setting we are working with", true, "classical, cl; r-motivic, rm; c-motivic, cm"};
        case cOPTION_TYPE_SPECIFIER_NONE:
            break;
    }
    return {"INVALID", "INVALID", false, "INVALID"};
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
        case cOPTION_VALUE_SETTING_C_MOTIVIC:
            return {"c-motivic", "cm"};
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

control_return_e argv_handle(option_values_t& aOptionValues, std::string& aCalculationInput, int argc, char** argv)
{
    option_type_specifier_e option_type_specifier;
    bool await_option_value = false;
    char* argv_i;
    for (int i = 1; i < argc; i++)
    {
        argv_i = argv[i];
        if (await_option_value)
        {
            await_option_value = false;
            // option_type_specifier will have been set from the previous iteration of the loop (for option types that expect values)
            switch (option_type_specifier)
            {
                case cOPTION_TYPE_SPECIFIER_SETTING:
                    aOptionValues.mSetting = option_value_setting_from_str(argv_i);
                    if (aOptionValues.mSetting == cOPTION_VALUE_SETTING_NONE)
                    {
                        option_type_details_t otd_setting = option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_SETTING);
                        std::cerr << "CLI input error: Unrecognized setting value given at index " << i << " (zero indexed): '" << argv_i << "'; valid value(s): '" << otd_setting.mValidValues << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
                        return cCONTROL_RETURN_FAIL;
                    }
                    break;
                default:
                    assert(!"unreachable"); // we only get here with a value required for the option, which should be handled by the previous cases
                    std::cerr << "[INTERNAL ERROR]: Unexpectedly reached theoretically unreachable code while handling CLI arguments" << std::endl;
                    return cCONTROL_RETURN_FAIL;
            }
        }
        else
        {
            if (is_token_option_specifier(argv_i))
            {
                argv_i += 2;
                option_type_specifier = option_type_specifier_from_str(argv_i);
                if (option_type_specifier == cOPTION_TYPE_SPECIFIER_NONE)
                {
                    std::cerr << "CLI input error: Unrecognized option given at index " << i << " (0 indexed): '--" << argv_i << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
                    return cCONTROL_RETURN_FAIL;
                }
                await_option_value = option_type_details_from_option_type_specifier(option_type_specifier).mExpectValue;
                if (!await_option_value)
                {
                    // no value required
                    switch (option_type_specifier)
                    {
                        case cOPTION_TYPE_SPECIFIER_VERSION:
                            std::cout << ADEMMA_VERSION_FULL << std::endl;
                            return cCONTROL_RETURN_SUCCESS_EXIT;
                        case cOPTION_TYPE_SPECIFIER_HELP:
                            print_help();
                            return cCONTROL_RETURN_SUCCESS_EXIT;
                        case cOPTION_TYPE_SPECIFIER_ONLY_CLI:
                            aOptionValues.mOnlyCLI = true;
                            break;
                        case cOPTION_TYPE_SPECIFIER_NO_LOOP_TUI:
                            aOptionValues.mNoLoopTUI = true;
                            break;
                        default:
                            assert(!"unreachable"); // we only get here with no value required for the option, which should be handled by the next iteration of the loop
                            std::cerr << "[INTERNAL ERROR]: Unexpectedly reached theoretically unreachable code while handling CLI arguments" << std::endl;
                            return cCONTROL_RETURN_FAIL;
                    }
                }
            }
            else
            {
                if (i + 1 != argc)
                {
                    std::cerr << "CLI input error: Non-option (options start with '--') given as not the last argument: '" << argv_i << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
                    return cCONTROL_RETURN_FAIL;
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
            std::cerr << "CLI input error: Unexpected end of arguments; value needed for option '--" << argv_i << "'; run '" << argv[0] << " --help' for syntax" << std::endl;
        }
        else
        {
            // while this case is not possible, I hate indexing into an array without knowing for certain that it's a valid index
            std::cerr << "CLI input error: Unexpected end of arguments; value needed for option '--" << argv_i << "'; run with '--help' for syntax" << std::endl;
        }
        return cCONTROL_RETURN_FAIL;
    }
    return cCONTROL_RETURN_SUCCESS;
}

bool is_token_option_specifier(const char* aToken)
{
    return (aToken[0] == '-' && aToken[1] == '-');
}

void print_help()
{
    std::cout << "usage: ademma";
    std::string options_str {};
    for (int ots_index = 0; ots_index < (int)cOPTION_TYPE_SPECIFIER_COUNT; ots_index++)
    {
        option_type_specifier_e ots_casted = (option_type_specifier_e)ots_index;
        option_type_details_t ot_details = option_type_details_from_option_type_specifier(ots_casted);
        options_str += std::string(" [--") + ot_details.mName + ((ot_details.mExpectValue) ? " <value>" : "") + "]";
    }
    options_str += " [calculation-input]\n";
    std::cout << options_str << std::endl;

    std::string descriptions_str = "descriptions:";
    for (int ots_index = 0; ots_index < (int)cOPTION_TYPE_SPECIFIER_COUNT; ots_index++)
    {
        descriptions_str += "\n\t";
        option_type_specifier_e ots_casted = (option_type_specifier_e)ots_index;
        option_type_details_t ot_details = option_type_details_from_option_type_specifier(ots_casted);
        descriptions_str += std::string(ot_details.mName) + " - " + ot_details.mDescription;
        if (!ot_details.mExpectValue)
        {
            continue;
        }
        descriptions_str += std::string("\n\t\tacceptable values: ") + ot_details.mValidValues;
    }
    std::cout << descriptions_str << "\n" << std::endl;

    std::cout << "calculation-input depends on the given '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_SETTING).mName << "' option, but most generally, Steenrod square degree factors can be specified as 'Sq^<num>' (with optional curly brackets '{' and '}' around '<num>'), tau factors can be specified as '\\tau' or '\\tau^<num>' (with optional curly brackets around '<num>'), and rho factors can be specified as '\\rho' or '\\rho^<num>' (with optional curly brackets around '<num>'). Classical setting only allows Steenrod square degrees; R-Motivic setting allows Steenrod square degrees, taus, and rhos. Multiplying the elements together is done by placing one element immediately after another without any characters between. Polynomial inputs are supported, so use the '+' character between monomials for your input. Whitespace is allowed between factors and between terms and '+'. Currently no parentheses are allowed. When passed via CLI, it is acceptable to use 'B' in place of backslashes '\\' but they will be converted before passing to parsing and calculations so error messages will show backslashes.\n" << std::endl;

    std::cout << "NOTE: If calculation-input is passed via CLI then, depending on the shell being used, a double backslash might be necessary for the shell to pass a backslash into the program argument. Usually shells allow arguments to be contained within double quotes, and when this is done, usually the shell will pass in a single backslash within those quotes as part of the argument instead of being treated as an escape character." << std::endl;
}

control_return_e handle_classical(const option_values_t& aOptionValues, const std::string& aCalculationInput)
{
    using namespace ademma_core;
    ParsingInfo parsing_info;
    parsing_info.mStringToParse = aCalculationInput;
    if (aCalculationInput.empty())
    {
        if (aOptionValues.mOnlyCLI)
        {
            std::cerr << "CLI input error: Option '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_ONLY_CLI).mName << "' was given but specifying the calculation input (as the last argument) is required (and was not given); run with '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_HELP).mName << "' for syntax" << std::endl;
            return cCONTROL_RETURN_FAIL;
        }
        std::cout << "Enter classical adem polynomial (form eg: Sq^4Sq^5+Sq^5) or 'q' to quit" << std::endl;
        std::getline(std::cin, parsing_info.mStringToParse);
        if (parsing_info.mStringToParse == "q")
        {
            return cCONTROL_RETURN_SUCCESS_EXIT;
        }
    }

    ClassicalAdemPolynomial user_input_cap = ClassicalAdemPolynomial_FromString(parsing_info);
    if (parsing_info.mErrorInfo.mIsError)
    {
        std::cerr << parsing_info.GetFullErrorString() << std::endl;
        return cCONTROL_RETURN_FAIL;
    }
    ClassicalAdemPolynomial cap_return = classical_adem_math::admissify_classical_adem_polynomial(user_input_cap);
    std::cout << ClassicalAdemPolynomial_ToString(cap_return) << std::endl;
    return cCONTROL_RETURN_SUCCESS;
}

control_return_e handle_r_motivic(const option_values_t& aOptionValues, const std::string& aCalculationInput)
{
    using namespace ademma_core;
    ParsingInfo parsing_info;
    parsing_info.mStringToParse = aCalculationInput;
    if (aCalculationInput.empty())
    {
        if (aOptionValues.mOnlyCLI)
        {
            std::cerr << "CLI input error: Option '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_ONLY_CLI).mName << "' was given but specifying the calculation input (as the last argument) is required (and was not given); run with '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_HELP).mName << "' for syntax" << std::endl;
            return cCONTROL_RETURN_FAIL;
        }
        std::cout << "Enter R-motivic adem polynomial (form eg: Sq^1\\tau\\rho^2Sq^2+Sq^3) or 'q' to quit" << std::endl;
        std::getline(std::cin, parsing_info.mStringToParse);
        if (parsing_info.mStringToParse == "q")
        {
            return cCONTROL_RETURN_SUCCESS_EXIT;
        }
    }

    RMotivicAdemPolynomial user_input_rmap = RMotivicAdemPolynomial_FromString(parsing_info);
    if (parsing_info.mErrorInfo.mIsError)
    {
        std::cerr << parsing_info.GetFullErrorString() << std::endl;
        return cCONTROL_RETURN_FAIL;
    }
    RMotivicAdemPolynomial rmap_return = r_motivic_adem_math::admissify_r_motivic_adem_polynomial(user_input_rmap);
    std::cout << RMotivicAdemPolynomial_ToString(rmap_return) << std::endl;
    return cCONTROL_RETURN_SUCCESS;
}

control_return_e handle_c_motivic(const option_values_t& aOptionValues, const std::string& aCalculationInput)
{
    using namespace ademma_core;
    ParsingInfo parsing_info;
    parsing_info.mStringToParse = aCalculationInput;
    if (aCalculationInput.empty())
    {
        if (aOptionValues.mOnlyCLI)
        {
            std::cerr << "CLI input error: Option '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_ONLY_CLI).mName << "' was given but specifying the calculation input (as the last argument) is required (and was not given); run with '--" << option_type_details_from_option_type_specifier(cOPTION_TYPE_SPECIFIER_HELP).mName << "' for syntax" << std::endl;
            return cCONTROL_RETURN_FAIL;
        }
        std::cout << "Enter C-motivic adem polynomial (form eg: Sq^1\\tauSq^2+Sq^3) or 'q' to quit" << std::endl;
        std::getline(std::cin, parsing_info.mStringToParse);
        if (parsing_info.mStringToParse == "q")
        {
            return cCONTROL_RETURN_SUCCESS_EXIT;
        }
    }

    CMotivicAdemPolynomial user_input_cmap = CMotivicAdemPolynomial_FromString(parsing_info);
    if (parsing_info.mErrorInfo.mIsError)
    {
        std::cerr << parsing_info.GetFullErrorString() << std::endl;
        return cCONTROL_RETURN_FAIL;
    }
    CMotivicAdemPolynomial cmap_return = c_motivic_adem_math::admissify_c_motivic_adem_polynomial(user_input_cmap);
    std::cout << CMotivicAdemPolynomial_ToString(cmap_return) << std::endl;
    return cCONTROL_RETURN_SUCCESS;
}

