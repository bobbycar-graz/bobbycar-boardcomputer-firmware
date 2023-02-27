#pragma once

// 3rdparty lib includes
#include <colorinterface.h>
#include <tftcolors.h>

// local includes
#include "globals.h"

namespace {
template<typename Tcontroller, int TsuccessColor>
class FeedbackColor : public virtual ColorInterface { public: int color() const { return Tcontroller::get().feedbackValid ? TsuccessColor : espgui::TFT_RED; } };

template<int TsuccessColor>
using FrontFeedbackColor = FeedbackColor<FrontControllerGetter, TsuccessColor>;

template<int TsuccessColor>
using BackFeedbackColor = FeedbackColor<BackControllerGetter, TsuccessColor>;
}
