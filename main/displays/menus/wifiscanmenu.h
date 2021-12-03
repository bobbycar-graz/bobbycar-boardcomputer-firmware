#pragma once

// local includes
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "displays/menus/stationwifisettingsmenu.h"
#include "menudisplay.h"
#include "texts.h"
#include "utils.h"

using namespace std::chrono_literals;
using namespace espgui;

class WifiScanMenu : public MenuDisplay, public BackActionInterface<SwitchScreenAction<StationWifiSettingsMenu>>
{
    using Base = MenuDisplay;
public:
    WifiScanMenu();
    std::string text() const override;
    void start() override;
    void update() override;
    void stop() override;
    void back() override;
private:
    std::optional<espchrono::millis_clock::time_point> m_lastScanComplete;
    std::vector<std::unique_ptr<makeComponent<MenuItem, ChangeableText, DummyAction>>> m_reusableItems;
};
