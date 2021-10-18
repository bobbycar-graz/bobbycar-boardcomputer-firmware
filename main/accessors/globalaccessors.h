#pragma once

// local includes
#include "accessorinterface.h"
#include "globals.h"
#include "modes/tempomatmode.h"

struct AvgSpeedAccessor : public espgui::RefAccessor<float> { float &getRef() const override { return avgSpeed; } };
struct AvgSpeedKmhAccessor : public espgui::RefAccessor<float> { float &getRef() const override { return avgSpeedKmh; } };
struct SumCurrentAccessor : public espgui::RefAccessor<float> { float &getRef() const override { return sumCurrent; } };

struct TempomatModeCruiseMotTgtAccessor : public espgui::RefAccessor<int16_t> { int16_t &getRef() const override { return modes::tempomatMode.nCruiseMotTgt; } };
