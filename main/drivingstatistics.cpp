#include "drivingstatistics.h"

// 3rdparty lib includes
#include <FastLED.h>
#include <TFT_eSPI.h>
#include <date/date.h>

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

void calculateStatistics()
{
    EVERY_N_MILLIS( 10 ) {
        static bool saveTotal = false;

        if ((settings.savedStatistics.totalCentimeters / 100.f) > drivingStatistics.totalMeters)
        {
            drivingStatistics.totalMeters = settings.savedStatistics.totalCentimeters / 100.f;
            drivingStatistics.last_cm_written = settings.savedStatistics.totalCentimeters;
        }

        static auto last_km_calculation = espchrono::millis_clock::now();
        const auto duration = espchrono::ago(last_km_calculation);
        last_km_calculation = espchrono::millis_clock::now();

        const float meters_driven_now = (abs(avgSpeedKmh) / 3.6) * (duration.count() / 1000.);
        drivingStatistics.meters_driven += meters_driven_now;
        drivingStatistics.totalMeters += meters_driven_now; // Udate meters driven

        if (abs(avgSpeedKmh) > 1)
        {
            if (!saveTotal && abs(avgSpeedKmh) > 5)
            {
                saveTotal = true;
            }
            drivingStatistics.currentDrivingTime += duration;

            float avgVoltage = 0;
            for (auto &controller : controllers)
            {
                avgVoltage += controller.getCalibratedVoltage();
            }
            avgVoltage = avgVoltage / controllers.size();

            auto watt = sumCurrent * avgVoltage;
            const float ws_driven_now = watt * (duration.count() / 1000.);
            drivingStatistics.wh_used += ws_driven_now / 3600; // Wh
            drivingStatistics.batteryWhEstimate -= ws_driven_now / 3600;
        }
        else
        {
            drivingStatistics.wh_used += (13 * (duration.count() / 1000.)) / 3600; // Wh
            drivingStatistics.batteryWhEstimate = getRemainingWattHours();
        }

        if ((drivingStatistics.totalMeters > ((drivingStatistics.last_cm_written / 100.f) + 100)) || (saveTotal && abs(avgSpeedKmh) < 0.5))
        {
            if (saveTotal)
            {
                saveTotal = false;
            }
            drivingStatistics.last_cm_written = drivingStatistics.totalMeters * 100; // Save total Meters
            settings.savedStatistics.totalCentimeters = drivingStatistics.last_cm_written;
            saveSettings();
        }
    }
}
