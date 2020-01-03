#pragma once

namespace mimax
{

template<typename TState, typename TTransition>
class IMinimaxResolver
{
public:
    virtual ~IMinimaxResolver() = default;
};

}