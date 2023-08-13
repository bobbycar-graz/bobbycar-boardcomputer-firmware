#include "speedinfodisplay.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <screenmanager.h>

// local includes
#include "displays/batteryinfodisplay.h"
#include "displays/menus/mainmenu.h"
#include "displays/statusdisplay.h"
#include "drivingstatistics.h"

void SpeedInfoDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    m_labelSpeed.start();

    m_dischargingBar.start();
    m_chargingBar.start();

    m_batteryPercentLabel.start();
    m_voltageLabel.start();
    m_distanceLabel.start();
    m_currentPowerLabel.start();
}

void SpeedInfoDisplay::redraw(espgui::TftInterface &tft)
{
    using namespace espgui;

    Base::redraw(tft);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(4);

    tft.setTextSize(4);

    m_labelSpeed.redraw(
            std::abs(avgSpeedKmh) < 10 ? fmt::format("{:.2f}", avgSpeedKmh) :
            (std::abs(avgSpeedKmh) < 100 ? fmt::format("{:.1f}", avgSpeedKmh) : fmt::format("{:.0f}", avgSpeedKmh)));

    tft.setTextSize(1);
    m_batteryPercentLabel.redraw(getBatteryPercentageString());

    if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
    {
        auto watt = sumCurrent * *avgVoltage;

        m_voltageLabel.redraw(fmt::format("{:.1f} V", avgVoltage.value()));
        tft.setTextSize(2);
        m_currentPowerLabel.redraw(fmt::format("{:.0f} W", watt));
        tft.setTextSize(1);
    }
    else
    {
        m_voltageLabel.redraw("No voltage");
        m_currentPowerLabel.redraw("No power");
    }

    m_distanceLabel.redraw(
            drivingStatistics.meters_driven > 1000 ? fmt::format("{:.3f} km", drivingStatistics.meters_driven / 1000) :
            (drivingStatistics.meters_driven > 100 ? fmt::format("{:.1f} m", drivingStatistics.meters_driven) : fmt::format("{:.2f} m", drivingStatistics.meters_driven)));

    tft.setTextSize(1);

    m_dischargingBar.redraw(sumCurrent < 0.f ? (-sumCurrent) : 0.f);
    m_chargingBar.redraw(sumCurrent > 0.f ? sumCurrent : 0.f);
}

void SpeedInfoDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Right:
        espgui::pushScreen<MainMenu>();
        break;
    case Button::Up:
        espgui::switchScreen<BatteryInfoDisplay>();
        break;
    case Button::Down:
#ifdef FEATURE_BMS
        espgui::switchScreen<BmsDisplay>();
#else
        espgui::switchScreen<StatusDisplay>();
#endif
        break;
    default:;
    }
}
