#include "metersdisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <fmt/core.h>

// local includes
#include "globals.h"
#include "utils.h"
#include "displays/menus/mainmenu.h"
#include "displays/statusdisplay.h"
#include "displays/bmsdisplay.h"

using namespace espgui;

void MetersDisplay::initScreen()
{
    Base::initScreen();

    tft.fillScreen(TFT_BLACK);

    m_vuMeter.start();

    m_dischargingBar.start();
    m_chargingBar.start();

    m_sumCurrentLabel.start();

    for (auto &meter : meters)
        meter.start();
}

void MetersDisplay::redraw()
{
    Base::redraw();

    m_vuMeter.redraw(avgSpeedKmh);

    m_dischargingBar.redraw(sumCurrent<0.f?(-sumCurrent):0.f);
    m_chargingBar.redraw(sumCurrent>0.f?sumCurrent:0.f);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(2);
    m_sumCurrentLabel.redraw(fmt::format("{:.02f}A", sumCurrent));

    meters[0].redraw(controllers.front.getCalibratedVoltage(), 35, 50);
    meters[1].redraw(controllers.back.getCalibratedVoltage(), 35, 50);
    meters[2].redraw(fixCurrent(controllers.front.feedback.left.dcLink), -10, 10);
    meters[3].redraw(fixCurrent(controllers.front.feedback.right.dcLink), -10, 10);
    meters[4].redraw(fixCurrent(controllers.back.feedback.left.dcLink), -10, 10);
    meters[5].redraw(fixCurrent(controllers.back.feedback.right.dcLink), -10, 10);
}

void MetersDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Right: pushScreen<MainMenu>(); break;
    case Button::Up: switchScreen<StatusDisplay>(); break;
    case Button::Down:
#ifdef FEATURE_BMS
        switchScreen<BmsDisplay>();
#else
        switchScreen<StatusDisplay>();
#endif
        break;
    default:;
    }
}
