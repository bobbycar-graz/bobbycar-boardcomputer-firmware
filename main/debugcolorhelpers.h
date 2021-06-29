#pragma once

#include "colorinterface.h"
#include "globals.h"

namespace {
template<typename Tcontroller, int TsuccessColor>
class FeedbackColor : public virtual ColorInterface { public: int color() const { return Tcontroller::get().feedbackValid ? TsuccessColor : TFT_RED; } };

template<int TsuccessColor>
using FrontFeedbackColor = FeedbackColor<FrontControllerGetter, TsuccessColor>;

template<int TsuccessColor>
using BackFeedbackColor = FeedbackColor<BackControllerGetter, TsuccessColor>;
}
