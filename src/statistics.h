#pragma once

#include "ring-buffer.h"

#include "globals.h"
#include "bmsutils.h"
#include "utils.h"

namespace {
namespace statistics {
using ContainerType = ring_buffer<float, 200>;
ContainerType gas, brems, avgSpeed, avgSpeedKmh, sumCurrent, sumAbsoluteCurrent
#ifdef GLUMP_CONTROLLER
  , frontVoltage, backVoltage, frontLeftCurrent, frontRightCurrent, backLeftCurrent, backRightCurrent
#endif

#ifdef FEATURE_BMS
, bmsVoltage, bmsCurrent, bmsPower
#endif
;
}

void pushStats()
{
    statistics::gas.push_back(gas);
    statistics::brems.push_back(brems);
    statistics::avgSpeed.push_back(avgSpeed);
    statistics::avgSpeedKmh.push_back(avgSpeedKmh);
    statistics::sumCurrent.push_back(sumCurrent);
    statistics::sumAbsoluteCurrent.push_back(sumAbsoluteCurrent);
#ifdef GLUMP_CONTROLLER
    if (front.feedbackValid)
    {
        statistics::frontVoltage.push_back(fixBatVoltage(front.feedback.batVoltage));
        statistics::frontLeftCurrent.push_back(fixCurrent(front.feedback.left.current));
        statistics::frontRightCurrent.push_back(fixCurrent(front.feedback.right.current));
    }
    if (back.feedbackValid)
    {
        statistics::backVoltage.push_back(fixBatVoltage(back.feedback.batVoltage));
        statistics::backLeftCurrent.push_back(fixCurrent(back.feedback.left.current));
        statistics::backRightCurrent.push_back(fixCurrent(back.feedback.right.current));
    }
#endif
#ifdef FEATURE_BMS
    statistics::bmsVoltage.push_back(bms::voltage);
    statistics::bmsCurrent.push_back(bms::current);
    statistics::bmsPower.push_back(bms::power);
#endif
}

class BufferAccessorInterface
{
public:
    virtual const statistics::ContainerType &getBuffer() const = 0;
};

template<const statistics::ContainerType &T>
class BufferAccessorImpl : public virtual BufferAccessorInterface
{
public:
    const statistics::ContainerType &getBuffer() const override { return T; }
};

using GasStatistics = BufferAccessorImpl<statistics::gas>;
using BremsStatistics = BufferAccessorImpl<statistics::brems>;
using AvgSpeedStatistics = BufferAccessorImpl<statistics::avgSpeed>;
using AvgSpeedKmhStatistics = BufferAccessorImpl<statistics::avgSpeedKmh>;
using SumCurrentStatistics = BufferAccessorImpl<statistics::sumCurrent>;
using SumAbsoluteCurrentStatistics = BufferAccessorImpl<statistics::sumAbsoluteCurrent>;
#ifdef GLUMP_CONTROLLER
using FrontVoltageStatistics = BufferAccessorImpl<statistics::frontVoltage>;
using BackVoltageStatistics = BufferAccessorImpl<statistics::backVoltage>;
#endif
#ifdef FEATURE_BMS
using BmsVoltageStatistics = BufferAccessorImpl<statistics::bmsVoltage>;
using BmsCurrentStatistics = BufferAccessorImpl<statistics::bmsCurrent>;
using BmsPowerStatistics = BufferAccessorImpl<statistics::bmsPower>;
#endif
#ifdef GLUMP_CONTROLLER
using FrontLeftCurrentStatistics = BufferAccessorImpl<statistics::frontLeftCurrent>;
using FrontRightCurrentStatistics = BufferAccessorImpl<statistics::frontRightCurrent>;
using BackLeftCurrentStatistics = BufferAccessorImpl<statistics::backLeftCurrent>;
using BackRightCurrentStatistics = BufferAccessorImpl<statistics::backRightCurrent>;
#endif
}
