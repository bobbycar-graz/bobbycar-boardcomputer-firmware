#include "drivingstatistics.h"

// 3rdparty lib includes
#include <FastLED.h>
#include <date/date.h>
#include <tftinterface.h>

// Local
#include "globals.h"
#include "battery.h"
#include "utils.h"

using namespace std::chrono_literals;

DrivingStatistics drivingStatistics;

float getAvgWhPerKm()
{
    return drivingStatistics.wh_used / (drivingStatistics.meters_driven / 1000.f);
}

float getAvgKmh()
{
    return (drivingStatistics.meters_driven / 1000.f) / ((drivingStatistics.currentDrivingTime / 1ms) / 1000.f / 60.f / 60.f); // (meter / 1000) / (ms / 1000 / 60 / 60)
}

float getEstimatedKmLeft()
{
    return (getRemainingWattHours() / getAvgWhPerKm());
}

std::string getEfficiencyClassString()
{
    const float avgWhPerKm = getAvgWhPerKm();
         if (avgWhPerKm <= 14) return "A+++";
    else if (avgWhPerKm <= 16) return "A++";
    else if (avgWhPerKm <= 18) return "A+";
    else if (avgWhPerKm <= 20) return "A";
    else if (avgWhPerKm <= 24) return "B";
    else if (avgWhPerKm <= 28) return "C";
    else if (avgWhPerKm <= 32) return "D";
    else if (avgWhPerKm <= 36) return "E";
    else if (avgWhPerKm <= 40) return "F";
    else                       return "G";
}

uint16_t getEfficiencyClassColor()
{
    const float avgWhPerKm = getAvgWhPerKm();
         if (avgWhPerKm <= 14) return 0x1700;
    else if (avgWhPerKm <= 16) return 0x3640;
    else if (avgWhPerKm <= 18) return 0x5560;
    else if (avgWhPerKm <= 20) return 0x6CA0;
    else if (avgWhPerKm <= 24) return 0x83E0;
    else if (avgWhPerKm <= 28) return 0x9B20;
    else if (avgWhPerKm <= 32) return 0xB240;
    else if (avgWhPerKm <= 36) return 0xC980;
    else if (avgWhPerKm <= 40) return 0xE0C0;
    else                       return 0xF800;
}

std::string getRemainingEstimateRangeString()
{
    return fmt::format("{:.1f} km", getEstimatedKmLeft());
}


void initStatistics()
{

}

void calculateStatistics()
{
    static bool saveTotal = false;

    if ((configs.savedStatistics.totalCentimeters.value() / 100.f) > drivingStatistics.totalMeters)
    {
        drivingStatistics.totalMeters = configs.savedStatistics.totalCentimeters.value() / 100.f;
        drivingStatistics.last_cm_written = configs.savedStatistics.totalCentimeters.value();
    }

    static auto last_km_calculation = espchrono::millis_clock::now();
    const auto duration = espchrono::ago(last_km_calculation);
    last_km_calculation = espchrono::millis_clock::now();

    const float meters_driven_now = (abs(avgSpeedKmh) / 3.6f) * ((duration / 1ms) / 1000.f);
    drivingStatistics.meters_driven += meters_driven_now;
    drivingStatistics.totalMeters += meters_driven_now; // Update meters driven

    if (abs(avgSpeedKmh) > 1)
    {
        if (!saveTotal && abs(avgSpeedKmh) > 5)
        {
            saveTotal = true;
        }
        drivingStatistics.currentDrivingTime += duration;

        if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
        {
            const float watt = sumCurrent * *avgVoltage;
            const float ws_driven_now = watt * ((duration / 1ms) / 1000.f);
            drivingStatistics.wh_used += ws_driven_now / 3600.f; // Wh
            drivingStatistics.batteryWhEstimate -= ws_driven_now / 3600.f;
        }
    }
    else
    {
        drivingStatistics.wh_used += (13 * ((duration / 1ms) / 1000.f)) / 3600.f; // Wh
        drivingStatistics.batteryWhEstimate = getRemainingWattHours();
    }

    if ((drivingStatistics.totalMeters > ((drivingStatistics.last_cm_written / 100.f) + 100)) || (saveTotal && abs(avgSpeedKmh) < 0.5f))
    {
        if (saveTotal)
        {
            saveTotal = false;
        }
        drivingStatistics.last_cm_written = drivingStatistics.totalMeters * 100; // Save total Meters
        if (!configs.emulateFeedback.value())
        {
            configs.write_config(configs.savedStatistics.totalCentimeters, drivingStatistics.last_cm_written);
        }
    }
}
