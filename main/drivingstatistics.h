#pragma once

// system includes
#include <chrono>
#include <string>

struct DrivingStatistics
{
  float meters_driven;
  std::chrono::milliseconds currentDrivingTime;
  double totalMeters;
  uint32_t last_cm_written;
  float wh_used;
  float batteryWhEstimate;
};

extern DrivingStatistics drivingStatistics;

void calculateStatistics();
float getAvgWhPerKm();
std::string getEfficiencyClassString();
uint16_t getEfficiencyClassColor();
