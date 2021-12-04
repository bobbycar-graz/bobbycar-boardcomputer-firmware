#pragma once

// 3rdparty lib includes
#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"

// local includes
#include "handbremse.h"

namespace espgui {
template<>
class ChangeValueDisplay<HandbremseMode> :
    public MenuDisplay,
    public virtual AccessorInterface<HandbremseMode>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    ChangeValueDisplay();

    void start() override;
};
} // namespace espgui
