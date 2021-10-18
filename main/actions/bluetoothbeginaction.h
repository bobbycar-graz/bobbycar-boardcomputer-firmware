#pragma once

// 3rdparty lib includes
#include "actioninterface.h"

#ifdef FEATURE_BLUETOOTH
class BluetoothBeginAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
#endif
