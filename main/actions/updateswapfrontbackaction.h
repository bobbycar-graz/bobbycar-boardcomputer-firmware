#pragma once

// 3rdparty lib includes
#include "actioninterface.h"

#ifdef FEATURE_SERIAL
class UpdateSwapFrontBackAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
#endif
