#pragma once

// 3rdparty lib includes
#include "actioninterface.h"

#ifdef FEATURE_BMS
class BmsTurnOffChargeAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
#endif
