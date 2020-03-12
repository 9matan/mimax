#pragma once

#include <limits>

namespace mimax
{

enum class EMinimaxOperation
{
    Max = 0,
    Min,
    Count
};

inline float GetDefaultScore(EMinimaxOperation const operation)
{
    return (operation == EMinimaxOperation::Min ? std::numeric_limits<float>::max() : -std::numeric_limits<float>::max());
}

inline bool CompareScore(float const lhsScore, float const rhsScore, EMinimaxOperation const operation)
{
    return (operation == EMinimaxOperation::Min ? lhsScore < rhsScore : lhsScore > rhsScore);
}

} // namespace mimax