#pragma once

#include "actioninterface.h"
#include "utils.h"

namespace {
#ifdef FEATURE_SERIAL
class UpdateSwapFrontBackAction : public virtual ActionInterface
{
public:
    void triggered() override { updateSwapFrontBack(); }
};
#endif
}
