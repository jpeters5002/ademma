#include "setting.hpp"

const char* ademma_core::str_from_Setting_Type(Setting_Type aSetting)
{
    switch (aSetting)
    {
        case Setting_Type::cCLASSICAL:
            return "classical";
        case Setting_Type::cC_MOTIVIC:
            return "c-motivic";
        case Setting_Type::cR_MOTIVIC:
            return "r-motivic";
    }
    return "[INVALID ENUM VALUE]";
}

