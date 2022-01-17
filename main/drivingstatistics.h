#pragma once

// system includes
#include <string>
#include <chrono>

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

void initStatistics();
void calculateStatistics();
float getAvgWhPerKm();
float getAvgKmh();
float getEstimatedKmLeft();
std::string getEfficiencyClassString();
std::string getRemainingEstimateRangeString();
uint16_t getEfficiencyClassColor();
