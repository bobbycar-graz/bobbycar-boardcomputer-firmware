#pragma once

// 3rdparty lib includes
#include "actioninterface.h"

#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
class BluetoothConnectBmsAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
#endif
