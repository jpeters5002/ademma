#ifndef CORE_INCLUDE_SETTING_HPP
#define CORE_INCLUDE_SETTING_HPP

namespace ademma_core
{
enum class Setting_Type
{
    cCLASSICAL,
    cC_MOTIVIC,
    cR_MOTIVIC,
};
const char* str_from_Setting_Type(Setting_Type aSetting);
}

#endif // CORE_INCLUDE_SETTING_HPP

