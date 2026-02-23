#include "general_adem_math.hpp"

#include <stdexcept>

ademma_uint_t ademma_core::general_adem_math::factorial_twos_count(ademma_uint_t aBase)
{
    ademma_uint_t runningCount = 0;
    ademma_uint_t lastCount = aBase;
    for (;;)
    {
        lastCount = lastCount / 2;
        if (lastCount == 0)
        {
            break;
        }
        runningCount += lastCount;
    }
    return runningCount;
}

bool ademma_core::general_adem_math::f2_choose(ademma_uint_t aTop, ademma_uint_t aBottom)
{
    if (aTop < aBottom)
    {
        throw std::runtime_error("f2_choose given aTop is less than given aBottom when it must be >=");
    }
    ademma_uint_t numeratorTwos = factorial_twos_count(aTop);
    ademma_uint_t denominatorTwos = factorial_twos_count(aBottom) + factorial_twos_count(aTop - aBottom);
    return numeratorTwos <= denominatorTwos;
}

