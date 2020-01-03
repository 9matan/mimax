#pragma once

#include "Minimax/IMinimaxResolver.h"

namespace mimax
{

template<typename TState, typename TTransition>
class CMinimaxAlgorithm
{
public:
    using IResolver = IMinimaxResolver<TState, TTransition>;
};

}