#include "bmsdisplay.h"

#if defined(FEATURE_BLUETOOTH) && defined(FEATURE_BMS)
#include "displays/menus/mainmenu.h"
#include "displays/metersdisplay.h"
#include "displays/statusdisplay.h"
#include "screenmanager.h"
#include "tftinstance.h"

using namespace espgui;

void BmsDisplay::initScreen()
{
    Base::initScreen();

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
    Base::redraw();

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
        m_voltageLabel.redraw(fmt::format("{:.02f}V", bms::voltage));
        m_capacityLabel.redraw(fmt::format("{:.02f} mAh", bms::capacity));
        m_socLabel.redraw(fmt::format("{:.02f}%", bms::soc));
        m_powerLabel.redraw(fmt::format("{:.02f}W", bms::power));
        m_currentLabel.redraw(fmt::format("{:.02f}A", bms::current));
    }
    else
    {
        m_voltageLabel.clear();
        m_capacityLabel.clear();
        m_socLabel.clear();
        m_powerLabel.clear();
        m_currentLabel.clear();
    }

    m_speedLabel.redraw(fmt::format("{:.02f}kmh", avgSpeedKmh));

    if (bluetoothSerial.hasClient())
        m_powerPerSpeedLabel.redraw(fmt::format("{:.02f}W/kmh", avgSpeedKmh < 1 ? 0 : bms::power / avgSpeedKmh));
    else
        m_powerPerSpeedLabel.clear();

    for (int i = 0; i < 12; i++)
        m_battLabels[i].redraw(fmt::format("{:.02f}", bms::batt[i]));

    if (bluetoothSerial.hasClient())
        m_cycleLabel.redraw(fmt::format("{:.02f}AH", bms::cycle));
    else
        m_cycleLabel.clear();
}

void BmsDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Right: pushScreen<MainMenu>(); break;
    case Button::Up: switchScreen<MetersDisplay>(); break;
    case Button::Down: switchScreen<StatusDisplay>(); break;
    default:;
    }
}
#endif
