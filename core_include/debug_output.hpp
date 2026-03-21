#ifndef CORE_INCLUDE_DEBUG_OUTPUT_HPP
#define CORE_INCLUDE_DEBUG_OUTPUT_HPP

#define DEBUG_OUTPUT 1
#if DEBUG_OUTPUT
#include <iostream>
#define DEBUG_PRINT(str) std::cout << "    DEBUG: " << str << std::endl
#else
#define DEBUG_PRINT(str) do{}while(0)
#endif

#endif // CORE_INCLUDE_DEBUG_OUTPUT_HPP

