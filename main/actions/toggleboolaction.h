#pragma once

#include "actioninterface.h"
#include "accessorinterface.h"

namespace {
class ToggleBoolAction : public virtual ActionInterface, public virtual AccessorInterface<bool>
{
public:
    void triggered() override
    {
        setValue(!getValue());
    }
};
}
