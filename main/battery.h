#pragma once

// system includes
#include <cstdint>

// 3rdparty lib includes
#include <cpptypesafeenum.h>

#define BatteryCellTypeValues(x) \
  x(_22P)                        \
      x(HG2)                     \
          x(MH1)                 \
              x(VTC5)            \
                  x(BAK_25R)
DECLARE_TYPESAFE_ENUM(BatteryCellType,
                      : uint8_t, BatteryCellTypeValues)

float getBatteryPercentage(float batVoltage, BatteryCellType cellType);

float getRemainingWattHours();

std::string getBatteryPercentageString();

std::string getBatteryRemainingWattHoursString();

std::string getBatteryCellTypeString();

std::string getRemainingRangeString();

std::string getBatteryDebugString();

std::string getBatteryAdvancedPercentageString();

float getPercentageByWh(float wh);
float getTarget_mAh();

namespace battery
{
extern float bootBatPercentage;
}
