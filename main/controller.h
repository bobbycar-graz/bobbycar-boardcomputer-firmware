#pragma once

// system includes
#include <functional>

// Arduino includes
#ifdef FEATURE_SERIAL
#include <HardwareSerial.h>
#endif

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "bobbycar-common.h"
#include "bobbycar-serial.h"
#ifdef FEATURE_SERIAL
#include "feedbackparser.h"
#endif

// forward declares
#ifdef FEATURE_SERIAL
class HardwareSerial;
#endif

namespace {
struct Controller {
    Controller(
#ifdef FEATURE_SERIAL
        HardwareSerial &serial,
#endif
        bool &enableLeft, bool &enableRight, bool &invertLeft, bool &invertRight) :
#ifdef FEATURE_SERIAL
        serial{serial},
#endif
        enableLeft{enableLeft}, enableRight{enableRight}, invertLeft{invertLeft}, invertRight{invertRight}
    {
    }
//    Controller(const Controller &) = delete;
//    Controller &operator=(const Controller &) = delete;

#ifdef FEATURE_SERIAL
    std::reference_wrapper<HardwareSerial> serial;
#endif
    bool &enableLeft, &enableRight, &invertLeft, &invertRight;

    bobbycar::protocol::serial::Command command{};

#ifdef FEATURE_CAN
    espchrono::millis_clock::time_point lastCanFeedback{};
#endif
    bool feedbackValid{};
    bobbycar::protocol::serial::Feedback feedback{};

#ifdef FEATURE_SERIAL
    FeedbackParser parser{serial, feedbackValid, feedback};
#endif
};
}
