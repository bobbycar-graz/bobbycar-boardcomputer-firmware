#pragma once

// 3rdparty lib includes
#include "graphdisplay.h"
#include <espwifistack.h>

// local includes
#include "globals.h"
#include "utils.h"

namespace statistics {
using ContainerType = ring_buffer<float, 200>;
extern ContainerType raw_gas, raw_brems, gas, brems, avgSpeed, avgSpeedKmh, sumCurrent, frontVoltage, backVoltage, frontLeftCurrent, frontRightCurrent, backLeftCurrent, backRightCurrent,
    rssi;
} // namespace statistics

void pushStats();

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

using RawGasStatistics = BufferAccessorImpl<statistics::raw_gas>;
using RawBremsStatistics = BufferAccessorImpl<statistics::raw_brems>;
using GasStatistics = BufferAccessorImpl<statistics::gas>;
using BremsStatistics = BufferAccessorImpl<statistics::brems>;
using AvgSpeedStatistics = BufferAccessorImpl<statistics::avgSpeed>;
using AvgSpeedKmhStatistics = BufferAccessorImpl<statistics::avgSpeedKmh>;
using SumCurrentStatistics = BufferAccessorImpl<statistics::sumCurrent>;
using FrontVoltageStatistics = BufferAccessorImpl<statistics::frontVoltage>;
using BackVoltageStatistics = BufferAccessorImpl<statistics::backVoltage>;
using FrontLeftCurrentStatistics = BufferAccessorImpl<statistics::frontLeftCurrent>;
using FrontRightCurrentStatistics = BufferAccessorImpl<statistics::frontRightCurrent>;
using BackLeftCurrentStatistics = BufferAccessorImpl<statistics::backLeftCurrent>;
using BackRightCurrentStatistics = BufferAccessorImpl<statistics::backRightCurrent>;
using RssiStatistics = BufferAccessorImpl<statistics::rssi>;
