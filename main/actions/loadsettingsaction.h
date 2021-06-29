#pragma once

#include "actioninterface.h"
#include "utils.h"

namespace {
class LoadSettingsAction : public virtual ActionInterface
{
public:
    void triggered() override { loadSettings(); }
};
}
