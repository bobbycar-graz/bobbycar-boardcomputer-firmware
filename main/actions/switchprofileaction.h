#pragma once

#include "actioninterface.h"
#include "settingsutils.h"

namespace {
template<uint8_t index>
class SwitchProfileAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        switchProfile(index);
    }
};
}
