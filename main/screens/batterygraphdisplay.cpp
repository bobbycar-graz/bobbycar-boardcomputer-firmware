#include "batterygraphdisplay.h"
constexpr const char * const TAG = "BatteryGraphDisplay";

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "battery.h"
#include "globals.h"
#include "newsettings.h"
#include "screens/batterymenu.h"
#include "utils.h"

namespace bobby {

namespace {
constexpr char TEXT_BATTERY_GRAPH[] = "Battery Level";
constexpr const uint8_t TOP_OFFSET = 40;
} // namespace

void BatteryGraphDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    drawBatteryCurve(tft);
}

std::string BatteryGraphDisplay::text() const
{
    if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
    {
        return fmt::format("{} ({:.1f}%)", TEXT_BATTERY_GRAPH, getBatteryPercentage(*avgVoltage, configs.battery.cellType.value()));
    }
    return TEXT_BATTERY_GRAPH;
}

void BatteryGraphDisplay::redraw(espgui::TftInterface &tft)
{
    using namespace espgui;
    Base::redraw(tft);

    if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
    {
        if (*avgVoltage == m_lastBatVoltage)
            return;
        const auto cellType = configs.battery.cellType.value();

        const float onePercent = (tft.width() - 4) / 100.f;
        const auto percentage = getBatteryPercentage(*avgVoltage, cellType);
        const uint16_t xOffset = onePercent * (100 - percentage);
        const uint16_t lastXOffset = onePercent * (100 - getBatteryPercentage(m_lastBatVoltage, cellType));

        // clear the old one and draw the new one
        tft.fillRect(lastXOffset + 2, TOP_OFFSET, onePercent, tft.height() - TOP_OFFSET, espgui::TFT_BLACK);
        tft.fillRect(xOffset + 2, TOP_OFFSET, onePercent, tft.height() - TOP_OFFSET, espgui::TFT_WHITE);
        m_lastBatVoltage = *avgVoltage;
        drawBatteryCurve(tft);
    }
    // tft.drawLine() code
}

void BatteryGraphDisplay::buttonPressed(espgui::Button button)
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

void BatteryGraphDisplay::drawBatteryCurve(espgui::TftInterface &tft)
{
    const auto points = count_curve_points(configs.battery.cellType.value());
    const auto max_height = tft.height() - 1;
    const auto max_width = tft.width() - 4;
    const uint16_t part = max_width / points;
    const auto min_voltage = getMinBatCellVoltage(configs.battery.cellType.value());
    const auto max_voltage = getMaxBatCellVoltage(configs.battery.cellType.value());
    for (uint8_t i = 0; points >= i; i++) {
        // draw lines between point->minVoltage and point->maxVoltage from left to right
        if (const auto point = get_point_n_voltages(configs.battery.cellType.value(), points - i); point)
        {
            const int x1 = 2 + part * (points - i + 1);
            const int y1 = float_map(point->minVoltage / 100.f, min_voltage, max_voltage, max_height, TOP_OFFSET);
            const int x2 = 2 + part * (points - i);
            const int y2 = float_map(point->maxVoltage / 100.f, min_voltage, max_voltage, max_height, TOP_OFFSET);
            tft.drawLine(x1, y1, x2, y2, espgui::TFT_WHITE);
        }
    }
}

} // namespace bobby
