#pragma once

#include "actioninterface.h"
#include "accessorinterface.h"

namespace {
template<typename T>
class SetValueAction : public virtual ActionInterface
{
public:
    SetValueAction(T value, AccessorInterface<T> &accessorInterface, BackInterface &backInterface) :
        m_value{value},
        m_accessorInterface{accessorInterface},
        m_backInterface{backInterface}
    {
    }

    void triggered() override
    {
        m_accessorInterface.setValue(m_value);
    }

    T value() const { return m_value; }

private:
    const T m_value;
    AccessorInterface<T> &m_accessorInterface;
    BackInterface &m_backInterface;
};
}
