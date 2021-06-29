#pragma once

#include "ring-buffer.h"

#include "globals.h"
#include "bmsutils.h"
#include "utils.h"

namespace {
namespace statistics {
using ContainerType = ring_buffer<float, 200>;
ContainerType gas, brems, avgSpeed, avgSpeedKmh, sumCurrent, frontVoltage, backVoltage, frontLeftCurrent, frontRightCurrent, backLeftCurrent, backRightCurrent
#ifdef FEATURE_BMS
, bmsVoltage, bmsCurrent, bmsPower
#endif
;
}

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
        statistics::frontVoltage.push_back(fixBatVoltage(controllers.front.feedback.batVoltage));
        statistics::frontLeftCurrent.push_back(fixCurrent(controllers.front.feedback.left.dcLink));
        statistics::frontRightCurrent.push_back(fixCurrent(controllers.front.feedback.right.dcLink));
    }
    if (controllers.back.feedbackValid)
    {
        statistics::backVoltage.push_back(fixBatVoltage(controllers.back.feedback.batVoltage));
        statistics::backLeftCurrent.push_back(fixCurrent(controllers.back.feedback.left.dcLink));
        statistics::backRightCurrent.push_back(fixCurrent(controllers.back.feedback.right.dcLink));
    }
#ifdef FEATURE_BMS
    statistics::bmsVoltage.push_back(bms::voltage);
    statistics::bmsCurrent.push_back(bms::current);
    statistics::bmsPower.push_back(bms::power);
#endif
}

class StatisticsAccessorInterface
{
public:
    virtual const statistics::ContainerType &getBuffer() const = 0;
};

template<const statistics::ContainerType &T>
class BufferAccessorImpl : public virtual StatisticsAccessorInterface
{
public:
    const statistics::ContainerType &getBuffer() const override { return T; }
};

using GasStatistics = BufferAccessorImpl<statistics::gas>;
using BremsStatistics = BufferAccessorImpl<statistics::brems>;
using AvgSpeedStatistics = BufferAccessorImpl<statistics::avgSpeed>;
using AvgSpeedKmhStatistics = BufferAccessorImpl<statistics::avgSpeedKmh>;
using SumCurrentStatistics = BufferAccessorImpl<statistics::sumCurrent>;
using FrontVoltageStatistics = BufferAccessorImpl<statistics::frontVoltage>;
using BackVoltageStatistics = BufferAccessorImpl<statistics::backVoltage>;
#ifdef FEATURE_BMS
using BmsVoltageStatistics = BufferAccessorImpl<statistics::bmsVoltage>;
using BmsCurrentStatistics = BufferAccessorImpl<statistics::bmsCurrent>;
using BmsPowerStatistics = BufferAccessorImpl<statistics::bmsPower>;
#endif
using FrontLeftCurrentStatistics = BufferAccessorImpl<statistics::frontLeftCurrent>;
using FrontRightCurrentStatistics = BufferAccessorImpl<statistics::frontRightCurrent>;
using BackLeftCurrentStatistics = BufferAccessorImpl<statistics::backLeftCurrent>;
using BackRightCurrentStatistics = BufferAccessorImpl<statistics::backRightCurrent>;
}
