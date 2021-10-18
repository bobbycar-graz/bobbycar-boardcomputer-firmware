#pragma once

// system includes
#include <cstdint>

// 3rdparty lib includes
#include "actioninterface.h"

template<uint8_t index>
class SwitchProfileAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
