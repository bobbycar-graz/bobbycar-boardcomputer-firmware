#pragma once

#include <functional>

#include <Arduino.h>
#include <WString.h>

#include "bobbycar-protocol/protocol.h"

#include "feedbackparser.h"
#include "VescUartControl-VESC6/VescUart.h"

class HardwareSerial;

namespace {
#ifdef GLUMP_CONTROLLER

struct Controller {
    Controller(HardwareSerial &serial, bool &enableLeft, bool &enableRight, bool &invertLeft, bool &invertRight) :
        serial{serial}, enableLeft{enableLeft}, enableRight{enableRight}, invertLeft{invertLeft}, invertRight{invertRight}
    {
    }

    std::reference_wrapper<HardwareSerial> serial;
    bool &enableLeft, &enableRight, &invertLeft, &invertRight;

    Command command{};

    bool feedbackValid{};
    Feedback feedback{};

    FeedbackParser parser{serial, feedbackValid, feedback};
};
#endif

#ifdef VESC_CONTROLLER
struct VescController {
  VescController(HardwareSerial &serial, bool &enable, bool &invert) :
        serial{serial}, enable{enable}, invert{invert}
    {
    }

    std::reference_wrapper<HardwareSerial> serial;
    bool &enable;
    bool &invert;

    bldcMeasure values;

    float pwm = 0;

//    Command command{};
//
//    bool feedbackValid{};
//    Feedback feedback{};
//
//    FeedbackParser parser{serial, feedbackValid, feedback};

    void update() {
      SetSerialPort(&serial.get());
      SetDebugSerialPort(NULL);

      VescUartGetValue(values);

      Serial.println(String{"Duty now "} + values.dutyNow);
    }
};
#endif
}
