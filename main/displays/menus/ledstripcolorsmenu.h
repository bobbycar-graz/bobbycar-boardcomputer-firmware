#pragma once

#include <TFT_eSPI.h>

// Local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "ledstrip.h"
#include "icons/back.h"
#include "icons/bobbycar.h"
#include "texts.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"

using namespace espgui;

namespace  {
    class LedstripMenu;
}

namespace  {
class LedstripColorsMenu :
        public MenuDisplay,
        public StaticText<TEXT_LEDSTRIPCOLORMENU>,
        public BackActionInterface<SwitchScreenAction<LedstripMenu>>
{
    using Base = MenuDisplay;
public:
    void redraw() override;
    LedstripColorsMenu()
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                         SwitchScreenAction<LedstripMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
    }
};

void LedstripColorsMenu::redraw()
{
    Base::redraw();
    tft.setSwapBytes(true);
    tft.pushImage(70, 80, bobbyicons::bobbycar.WIDTH, bobbyicons::bobbycar.HEIGHT, bobbyicons::bobbycar.buffer);
    tft.setSwapBytes(false);
}

} // Namespace
