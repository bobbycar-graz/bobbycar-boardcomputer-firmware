#pragma once

// 3rdparty lib includes
#include <actioninterface.h>

namespace bobby {

using namespace espgui;

class ModeSettingsAction : public virtual ActionInterface
{
public:
    void triggered() override;
};
} // namespace bobby
