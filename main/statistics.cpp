#include "statistics.h"

namespace statistics
{
ContainerType gas, brems, avgSpeed, avgSpeedKmh, sumCurrent, frontVoltage, backVoltage, frontLeftCurrent, frontRightCurrent, backLeftCurrent, backRightCurrent,
#ifdef FEATURE_BMS
    bmsVoltage, bmsCurrent, bmsPower,
#endif
    rssi;
} // namespace statistics

void pushStats()
{
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
#ifdef FEATURE_BMS
    statistics::bmsVoltage.push_back(bms::voltage);
    statistics::bmsCurrent.push_back(bms::current);
    statistics::bmsPower.push_back(bms::power);
#endif
    if (wifi_stack::get_sta_status() == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        if (const auto &result = wifi_stack::get_sta_ap_info(); result)
            statistics::rssi.push_back(result->rssi);
    }
}
