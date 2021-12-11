#include "battery.h"

// 3rdparty lib includes
#include <cpputils.h>
#include <fmt/core.h>

// local includes
#include "drivingstatistics.h"
#include "globals.h"

#define CURVE(higherVoltage, lowerVoltage, fromAh, toAh)             \
    if (cellVoltage >= lowerVoltage && cellVoltage <= higherVoltage) \
        return 100 * (expected_ah - cpputils::mapValue<float>(cellVoltage, higherVoltage, lowerVoltage, fromAh, toAh)) / expected_ah;

float getBatteryPercentage(float batVoltage, BatteryCellType cellType)
{
    const float cellVoltage = batVoltage / settings.battery.cellsSeries;

    switch (cellType)
    {
    case BatteryCellType::_22P:
    {
        const float expected_ah = 2.2;
        if (cellVoltage > 4.15)
            return 100;

        CURVE(4.15, 4.04, 0, 0.25)
        CURVE(4.04, 3.95, 0.25, 0.5)
        CURVE(3.95, 3.86, 0.5, 0.75)
        CURVE(3.86, 3.74, 0.75, 1.0)
        CURVE(3.74, 3.64, 1.0, 1.25)
        CURVE(3.64, 3.59, 1.25, 1.5)
        CURVE(3.59, 3.54, 1.5, 1.75)
        CURVE(3.54, 3.43, 1.75, 2.0)
        CURVE(3.43, 3.35, 2.0, 2.1)
        CURVE(3.35, 2.50, 2.1, 2.2)
        break;
    }
    case BatteryCellType::MH1:
    {
        const float expected_ah = 3.2;
        if (cellVoltage > 4.15)
            return 100;

        CURVE(4.15, 4.09, 0, 0.25)
        CURVE(4.09, 4.04, 0.25, 0.5)
        CURVE(4.04, 3.95, 0.5, 0.75)
        CURVE(3.95, 3.88, 0.75, 1.0)
        CURVE(3.88, 3.79, 1.0, 1.25)
        CURVE(3.79, 3.70, 1.25, 1.5)
        CURVE(3.70, 3.65, 1.5, 1.75)
        CURVE(3.65, 3.60, 1.75, 2.0)
        CURVE(3.60, 3.56, 2.0, 2.25)
        CURVE(3.56, 3.50, 2.25, 2.5)
        CURVE(3.50, 3.40, 2.5, 2.75)
        CURVE(3.40, 3.30, 2.75, 3.0)
        CURVE(3.30, 2.5, 3.0, 3.2)
        break;
    }
    case BatteryCellType::HG2:
    {
        const float expected_ah = 3.0;
        if (cellVoltage > 4.15)
            return 100;

        CURVE(4.15, 4.08, 0, 0.25)
        CURVE(4.08, 4.01, 0.25, 0.5)
        CURVE(4.01, 3.92, 0.5, 0.75)
        CURVE(3.92, 3.84, 0.75, 1.0)
        CURVE(3.84, 3.75, 1.0, 1.25)
        CURVE(3.75, 3.67, 1.25, 1.5)
        CURVE(3.67, 3.62, 1.5, 1.75)
        CURVE(3.62, 3.55, 1.75, 2.0)
        CURVE(3.55, 3.44, 2.0, 2.25)
        CURVE(3.44, 3.30, 2.25, 2.5)
        CURVE(3.30, 3.05, 2.5, 2.75)
        CURVE(3.05, 2.50, 2.75, 3.0)
        break;
    }
    case BatteryCellType::VTC5:
    {
        const float expected_ah = 2.6;
        if (cellVoltage > 4.15)
            return 100;

        CURVE(4.15, 4.08, 0, 0.25)
        CURVE(4.08, 3.98, 0.25, 0.5)
        CURVE(3.98, 3.89, 0.5, 0.75)
        CURVE(3.89, 3.79, 0.75, 1.0)
        CURVE(3.79, 3.71, 1.0, 1.25)
        CURVE(3.71, 3.64, 1.25, 1.5)
        CURVE(3.64, 3.53, 1.5, 1.75)
        CURVE(3.53, 3.44, 1.75, 2.0)
        CURVE(3.44, 3.20, 2.0, 2.25)
        CURVE(3.20, 2.80, 2.25, 2.5)
        CURVE(2.80, 2.50, 2.5, 2.60)
        break;
    }
    case BatteryCellType::BAK_25R:
    {
        const float expected_ah = 2.5;
        if (cellVoltage > 4.15)
        {
            return 100;
        }
        CURVE(4.15, 4.06, 0, 0.25)
        CURVE(4.06, 3.96, 0.25, 0.5)
        CURVE(3.96, 3.88, 0.5, 0.75)
        CURVE(3.88, 3.77, 0.75, 1)
        CURVE(3.77, 3.68, 1, 1.25)
        CURVE(3.68, 3.62, 1.25, 1.5)
        CURVE(3.62, 3.56, 1.5, 1.75)
        CURVE(3.56, 3.47, 1.75, 2)
        CURVE(3.47, 3.31, 2, 2.25)
        CURVE(3.31, 2.50, 2.25, 2.5)
        break;
    }
    }
    return 0.f;
}

float getRemainingWattHours()
{
    float target_mah = getTarget_mAh();

    float avgVoltage = 0;
    for (auto &controller : controllers)
    {
        avgVoltage += controller.getCalibratedVoltage();
    }
    avgVoltage = avgVoltage / controllers.size();

    return (target_mah / 1000.f) * 3.7 * settings.battery.cellsParallel * settings.battery.cellsSeries * (getBatteryPercentage(avgVoltage, BatteryCellType(settings.battery.cellType)) / 100);
}

float getPercentageByWh(float wh)
{
    const float maxWh = (getTarget_mAh() / 1000.f) * 3.7 * settings.battery.cellsParallel * settings.battery.cellsSeries;
    return maxWh / wh;
}

float getTarget_mAh()
{
    float target_mah = 2000; // default
    if (BatteryCellType(settings.battery.cellType) == BatteryCellType::_22P) target_mah = 2200;
    if (BatteryCellType(settings.battery.cellType) == BatteryCellType::HG2) target_mah = 3000;
    if (BatteryCellType(settings.battery.cellType) == BatteryCellType::MH1) target_mah = 3200;
    if (BatteryCellType(settings.battery.cellType) == BatteryCellType::VTC5) target_mah = 2600;
    if (BatteryCellType(settings.battery.cellType) == BatteryCellType::BAK_25R) target_mah = 2500;
    return target_mah;
}

std::string getBatteryPercentageString()
{
    float avgVoltage = 0;
    for (auto &controller : controllers)
    {
        avgVoltage += controller.getCalibratedVoltage();
    }
    avgVoltage = avgVoltage / controllers.size();

    std::string output = fmt::format("Battery: {:.1f}%", getBatteryPercentage(avgVoltage, BatteryCellType(settings.battery.cellType)));
    return output;
}

std::string getBatteryAdvancedPercentageString()
{
    std::string output = fmt::format("Battery: {:.1f}%", getPercentageByWh(drivingStatistics.batteryWhEstimate));
    return output;
}

std::string getBatteryRemainingWattHoursString()
{
    return fmt::format("{:.1f}Wh", getRemainingWattHours());
}

std::string getBatteryCellTypeString()
{
    return fmt::format("Cells: {}", toString(BatteryCellType(settings.battery.cellType)));
}

std::string getRemainingRangeString()
{
    return fmt::format("{:.1f} km", getRemainingWattHours() / settings.battery.watthoursPerKilometer);
}

std::string getBatteryDebugString()
{
    float avgVoltage = 0;
    for (auto &controller : controllers)
    {
        avgVoltage += controller.getCalibratedVoltage();
    }
    avgVoltage = avgVoltage / controllers.size();
    return fmt::format("{:.1f}V {}A", avgVoltage, sumCurrent);
}

namespace battery
{
float bootBatPercentage { -1 };
}
