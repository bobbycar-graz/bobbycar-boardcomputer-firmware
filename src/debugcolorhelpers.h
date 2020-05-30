#pragma once

#include "colorinterface.h"
#include "globals.h"

#ifdef GLUMP_CONTROLLER
namespace {
template<Controller &Tcontroller, int TsuccessColor>
class FeedbackColor : public virtual ColorInterface { public: int color() const { return Tcontroller.feedbackValid ? TsuccessColor : TFT_RED; } };

template<int TsuccessColor>
using FrontFeedbackColor = FeedbackColor<front, TsuccessColor>;

template<int TsuccessColor>
using BackFeedbackColor = FeedbackColor<back, TsuccessColor>;
}
#endif
