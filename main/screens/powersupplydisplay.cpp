#include "powersupplydisplay.h"

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "globals.h"

#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)

namespace bobby {

void PowerSupplyDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.drawString("Voltage:", 0, m_voltageLabel.y(), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_voltageLabel.start(tft);
    tft.drawString("Current:", 0, m_currentLabel.y(), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_currentLabel.start(tft);
}

void PowerSupplyDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    m_voltageLabel.redraw(tft, std::to_string(50.4) + 'V', espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_currentLabel.redraw(tft, std::to_string(15.1) + 'A', espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
}

void PowerSupplyDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Left:
    case Button::Right:
        espgui::popScreen();
        break;
    default:;
    }
}
} // namespace bobby
#endif
