#pragma once

#include "actioninterface.h"
#include "utils.h"

namespace {
class SaveSettingsAction : public virtual ActionInterface
{
public:
    void triggered() override { saveSettings(); }
};
}
