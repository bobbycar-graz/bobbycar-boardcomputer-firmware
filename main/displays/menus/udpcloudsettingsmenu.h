#pragma once

// 3rdparty lib includes
#include "menudisplay.h"

// local includes
#include "texts.h"

#ifdef FEATURE_UDPCLOUD
class UdpCloudSettingsMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_UDPCLOUDSETTINGS>
{
public:
    UdpCloudSettingsMenu();

    void back() override;
};
#endif
