#pragma once

#include <Arduino.h>

#include "display.h"
#include "actions/switchscreenaction.h"
#include "globals.h"
#include "bmsutils.h"
#include "widgets/label.h"

namespace {
class MainMenu;
class MetersDisplay;
class StatusDisplay;
}
namespace {
#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
class BmsDisplay : public Display, public ConfirmActionInterface<SwitchScreenAction<MainMenu>>, public DummyBack
{
public:
    void initScreen() override;
    void redraw() override;

    void rotate(int offset) override;

    Label m_statusLabel{200, 0};

    Label m_voltageLabel{107, 0};
    Label m_capacityLabel{107, 25};
    Label m_socLabel{107, 50};
    Label m_powerLabel{107, 75};
    Label m_currentLabel{107, 100};
    Label m_speedLabel{107, 125};
    Label m_powerPerSpeedLabel{107, 150};

    std::array<Label, 12> m_battLabels{{
        Label{0, 225}, Label{60, 225}, Label{120, 225}, Label{180, 225},
        Label{0, 250}, Label{60, 250}, Label{120, 250}, Label{180, 250},
        Label{0, 275}, Label{60, 275}, Label{120, 275}, Label{180, 275}
    }};

    Label m_cycleLabel{105, 300};
};

void BmsDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.setTextFont(2);
    m_statusLabel.start();
    m_statusLabel.redraw("init");

    tft.setTextFont(4);
    tft.drawString("Voltage:", 0, m_voltageLabel.y());
    m_voltageLabel.start();
    tft.drawString("Capacity:", 0, m_capacityLabel.y());
    m_capacityLabel.start();
    tft.drawString("SOC:", 0, m_socLabel.y());
    m_socLabel.start();
    tft.drawString("Power:", 0, m_powerLabel.y());
    m_powerLabel.start();
    tft.drawString("Current:", 0, m_currentLabel.y());
    m_currentLabel.start();
    tft.drawString("Speed:", 0, m_speedLabel.y());
    m_speedLabel.start();
    tft.drawString("PpS:", 0, m_powerPerSpeedLabel.y());
    m_powerPerSpeedLabel.start();

    for (auto &label : m_battLabels)
        label.start();

    tft.drawString("Cycle:", 0, m_cycleLabel.y());
    m_cycleLabel.start();
}

void BmsDisplay::redraw()
{
    if (bluetoothSerial.hasClient())
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    else
    {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setTextFont(2);
    }

    m_statusLabel.redraw(bluetoothSerial.hasClient() ? "OK" : "FAIL");

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    if (!bluetoothSerial.hasClient())
        tft.setTextFont(4);

    if (bluetoothSerial.hasClient())
    {
        m_voltageLabel.redraw(std::to_string(bms::voltage) + 'V');
        m_capacityLabel.redraw(std::to_string(int(bms::capacity)) + "mAh");
        m_socLabel.redraw(std::to_string(bms::soc) + '%');
        m_powerLabel.redraw(std::to_string(bms::power) + 'W');
        m_currentLabel.redraw(std::to_string(bms::current) + 'A');
    }
    else
    {
        m_voltageLabel.clear();
        m_capacityLabel.clear();
        m_socLabel.clear();
        m_powerLabel.clear();
        m_currentLabel.clear();
    }

    m_speedLabel.redraw(std::to_string(avgSpeedKmh) + "kmh");

    if (bluetoothSerial.hasClient())
        m_powerPerSpeedLabel.redraw(std::to_string(avgSpeedKmh < 1 ? 0 : bms::power / avgSpeedKmh) + "W/kmh");
    else
        m_powerPerSpeedLabel.clear();

    for (int i = 0; i < 12; i++)
        m_battLabels[i].redraw(std::to_string(bms::batt[i]));

    if (bluetoothSerial.hasClient())
        m_cycleLabel.redraw(std::to_string(bms::cycle) + "AH");
    else
        m_cycleLabel.clear();
}

void BmsDisplay::rotate(int offset)
{
    if (offset < 0)
        switchScreen<MetersDisplay>();
    else if (offset > 0)
        switchScreen<StatusDisplay>();
}
#endif
}
