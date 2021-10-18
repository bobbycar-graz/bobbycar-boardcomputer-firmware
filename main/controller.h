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

struct Controller
{
    Controller(
#ifdef FEATURE_SERIAL
        HardwareSerial &serial,
#endif
        bool &enableLeft, bool &enableRight, bool &invertLeft, bool &invertRight,
        int16_t &voltageCalib30V, int16_t &voltageCalib50V) :
#ifdef FEATURE_SERIAL
        serial{serial},
#endif
        enableLeft{enableLeft}, enableRight{enableRight}, invertLeft{invertLeft}, invertRight{invertRight},
        voltageCalib30V{voltageCalib30V}, voltageCalib50V{voltageCalib50V}
    {
    }
//    Controller(const Controller &) = delete;
//    Controller &operator=(const Controller &) = delete;

#ifdef FEATURE_SERIAL
    std::reference_wrapper<HardwareSerial> serial;
#endif
    bool &enableLeft, &enableRight, &invertLeft, &invertRight;
    int16_t &voltageCalib30V, &voltageCalib50V;

    bobbycar::protocol::serial::Command command{};

#ifdef FEATURE_CAN
    espchrono::millis_clock::time_point lastCanFeedback{};
#endif
    bool feedbackValid{};
    bobbycar::protocol::serial::Feedback feedback{};


#ifdef FEATURE_SERIAL
    FeedbackParser parser{serial, feedbackValid, feedback};
#endif

    float getCalibratedVoltage(bool applyCalibration) const
    {
        float voltage = feedback.batVoltage;
        if (applyCalibration)
        {
            voltage = ((voltage - float(voltageCalib30V)) * (20.f / (float(voltageCalib50V) - float(voltageCalib30V))) + 30.f);
        }
        else
        {
            voltage = voltage / 100.;
        }
        return voltage;
    }
};
