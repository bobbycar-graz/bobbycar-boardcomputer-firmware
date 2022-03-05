#include "powersupplydisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>

// local includes
#include "globals.h"

#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)
void PowerSupplyDisplay::initScreen()
{
    Base::initScreen();

    espgui::tft.fillScreen(TFT_BLACK);
    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    espgui::tft.setTextFont(4);

    espgui::tft.drawString("Voltage:", 0, m_voltageLabel.y());
    m_voltageLabel.start();
    espgui::tft.drawString("Current:", 0, m_currentLabel.y());
    m_currentLabel.start();
}

void PowerSupplyDisplay::redraw()
{
    Base::redraw();

    m_voltageLabel.redraw(std::to_string(50.4) + 'V');
    m_currentLabel.redraw(std::to_string(15.1) + 'A');
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
#endif
