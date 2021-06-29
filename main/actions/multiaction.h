#pragma once

#include "actioninterface.h"

namespace {
template<typename ...T>
class MultiAction;

template<typename T>
class MultiAction<T> : public virtual ActionInterface
{
public:
    void triggered() override
    {
        T{}.triggered();
    }
};

template<typename T, typename ...Tmore>
class MultiAction<T, Tmore...> : public virtual MultiAction<Tmore...>
{
public:
    void triggered() override
    {
        T{}.triggered();
        MultiAction<Tmore...>::triggered();
    }
};
}
