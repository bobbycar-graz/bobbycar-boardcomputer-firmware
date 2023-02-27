#pragma once

// system includes
#include <esp_system.h>

// 3rdparty lib includes
#include <actioninterface.h>


// local includes
#include "newsettings.h"

class ResetNVSAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        configs.reset();
    }
};
