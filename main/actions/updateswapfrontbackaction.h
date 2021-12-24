#pragma once

#include "actioninterface.h"
#include "utilities/utils.h"

using namespace espgui;

namespace {
#ifdef FEATURE_SERIAL
class UpdateSwapFrontBackAction : public virtual ActionInterface
{
public:
    void triggered() override { updateSwapFrontBack(); }
};
#endif
}
