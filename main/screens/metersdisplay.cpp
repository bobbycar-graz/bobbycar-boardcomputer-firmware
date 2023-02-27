#include "metersdisplay.h"

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "globals.h"
#include "screens/batteryinfodisplay.h"
#include "screens/mainmenu.h"
#include "screens/statusdisplay.h"
#include "utils.h"

namespace bobby {

using namespace espgui;

void MetersDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.fillScreen(espgui::TFT_BLACK);

    m_vuMeter.start(tft);

    m_dischargingBar.start(tft);
    m_chargingBar.start(tft);

    m_sumCurrentLabel.start(tft);

    for (auto &meter : meters)
        meter.start(tft);
}

void MetersDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    m_vuMeter.redraw(tft, avgSpeedKmh);

    m_dischargingBar.redraw(tft, sumCurrent<0.f?(-sumCurrent):0.f);
    m_chargingBar.redraw(tft, sumCurrent>0.f?sumCurrent:0.f);

    m_sumCurrentLabel.redraw(tft, fmt::format("{:.02f}A", sumCurrent), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);

    meters[0].redraw(tft, controllers.front.getCalibratedVoltage(), 35, 50);
    meters[1].redraw(tft, controllers.back.getCalibratedVoltage(), 35, 50);
    meters[2].redraw(tft, fixCurrent(controllers.front.feedback.left.dcLink), -10, 10);
    meters[3].redraw(tft, fixCurrent(controllers.front.feedback.right.dcLink), -10, 10);
    meters[4].redraw(tft, fixCurrent(controllers.back.feedback.left.dcLink), -10, 10);
    meters[5].redraw(tft, fixCurrent(controllers.back.feedback.right.dcLink), -10, 10);
}

void MetersDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Right:
        pushScreen<MainMenu>();
        break;
    case Button::Up:
        switchScreen<StatusDisplay>();
        break;
    case Button::Down:
        switchScreen<BatteryInfoDisplay>();
        break;
    default:;
    }
}
} // namespace bobby
