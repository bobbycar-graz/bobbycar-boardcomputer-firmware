#include "statistics.h"

namespace statistics {
ContainerType raw_gas, raw_brems, gas, brems, avgSpeed, avgSpeedKmh, sumCurrent, frontVoltage, backVoltage, frontLeftCurrent, frontRightCurrent, backLeftCurrent, backRightCurrent,
    rssi;
} // namespace statistics

void pushStats()
{
    if (raw_gas)
        statistics::raw_gas.push_back(*raw_gas);
    if (raw_brems)
        statistics::raw_brems.push_back(*raw_brems);
    if (gas)
        statistics::gas.push_back(*gas);
    if (brems)
        statistics::brems.push_back(*brems);
    statistics::avgSpeed.push_back(avgSpeed);
    statistics::avgSpeedKmh.push_back(avgSpeedKmh);
    statistics::sumCurrent.push_back(sumCurrent);
    if (controllers.front.feedbackValid)
    {
        statistics::frontVoltage.push_back(controllers.front.getCalibratedVoltage());
        statistics::frontLeftCurrent.push_back(fixCurrent(controllers.front.feedback.left.dcLink));
        statistics::frontRightCurrent.push_back(fixCurrent(controllers.front.feedback.right.dcLink));
    }
    if (controllers.back.feedbackValid)
    {
        statistics::backVoltage.push_back(controllers.back.getCalibratedVoltage());
        statistics::backLeftCurrent.push_back(fixCurrent(controllers.back.feedback.left.dcLink));
        statistics::backRightCurrent.push_back(fixCurrent(controllers.back.feedback.right.dcLink));
    }
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            statistics::rssi.push_back(result->rssi);
    }
}
