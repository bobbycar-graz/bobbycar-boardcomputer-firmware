#include "batteryinfodisplay.h"

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftinstance.h>

// local includes
#include "battery.h"
#include "displays/menus/mainmenu.h"
#include "displays/speedinfodisplay.h"
#include "displays/statusdisplay.h"

// display with big battery and ten bars (0-100%)

void BatteryInfoDisplay::initScreen()
{
    using namespace espgui;
    Base::initScreen();

    tft.drawRoundRect(m_offset, m_offset, tft.width() - (m_offset * 2), tft.height() - (m_offset * 2), 10, TFT_WHITE);
    tft.drawRoundRect((tft.width() / 2) - (m_offset / 2), m_offset / 2, m_offset, m_offset / 2, 3, TFT_WHITE);
    m_lastBarCount = 0;
}

void BatteryInfoDisplay::redraw()
{
    using namespace espgui;
    Base::redraw();

    // calculate height of space available for all bars
    const auto min_x = m_offset + 3; // leave 2 pixels + 1 pixel for border
    const auto max_x = tft.width() - m_offset - 3;
    const auto topY = m_offset + 3;
    const auto bottomY = tft.height() - m_offset - 3;
    const auto height = bottomY - topY;
    const auto width = max_x - min_x;
    const uint16_t segment_height = (height / 10);

    if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
    {
        const auto cellType = configs.battery.cellType.value();
        const uint16_t percentage = getBatteryPercentage(*avgVoltage, cellType);
        const auto segment_count = std::max(percentage / 10, 1);

        if (segment_count != m_lastBarCount)
        {
            m_lastBarCount = segment_count;
            // draw battery
            for (auto i = 0; i < 10; ++i)
            {
                const auto y = bottomY - (i * segment_height) - segment_height;
                tft.fillRoundRect(min_x, y, width, segment_height - 2, 10, segment_count > i ? TFT_GREEN : TFT_DARKGREY);
            }
        }
    }

    // tft.fillRect(0, 0, tft.width(), topY, TFT_CYAN);
    // tft.fillRect(0, bottomY, tft.width(), tft.height()-bottomY, TFT_YELLOW);
}

void BatteryInfoDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button) {
        using espgui::Button;
        case Button::Right:
            espgui::pushScreen<MainMenu>();
            break;
        case Button::Up:
            espgui::switchScreen<StatusDisplay>();
            break;
        case Button::Down:
            espgui::switchScreen<SpeedInfoDisplay>();
            break;
        default:;
    }
}
