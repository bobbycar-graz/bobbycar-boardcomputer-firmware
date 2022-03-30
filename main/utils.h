#pragma once

// system includes
#include <algorithm>
#include <utility>
#include <string>

// Arduino includes
#ifdef FEATURE_SERIAL
#include <HardwareSerial.h>
#endif

// 3rdparty lib includes
#include <espchrono.h>
#include <futurecpp.h>
#include <cpputils.h>

// local includes
#ifdef FEATURE_CAN
#include "bobbycar-can.h"
#endif
#ifdef FEATURE_SERIAL
#include "bobbycar-serial.h"
#endif

#include "display.h"
#include "globals.h"
#ifdef FEATURE_CAN
#include "can.h"
#endif

extern bool currentlyReverseBeeping;
extern bool reverseBeepToggle;
extern espchrono::millis_clock::time_point lastReverseBeepToggle;

float convertToKmh(float val);
float convertFromKmh(float val);
float convertToInch(float val);
float convertFromInch(float val);
float fixCurrent(int16_t value);
float fixBoardTemp(int16_t value);
std::string hallString(const bobbycar::protocol::serial::MotorFeedback &motor);
std::string to_string(bobbycar::protocol::ControlType value);
std::string to_string(bobbycar::protocol::ControlMode value);
std::array<std::reference_wrapper<bobbycar::protocol::serial::MotorState>, 2> motorsInController(Controller &controller);
std::array<std::reference_wrapper<const bobbycar::protocol::serial::MotorState>, 2> motorsInController(const Controller &controller);
std::array<std::reference_wrapper<bobbycar::protocol::serial::MotorFeedback>, 2> motorFeedbacksInController(Controller &controller);
std::array<std::reference_wrapper<const bobbycar::protocol::serial::MotorFeedback>, 2> motorFeedbacksInController(const Controller &controller);
std::array<std::reference_wrapper<bobbycar::protocol::serial::MotorState>, 4> motors();
void fixCommonParams();
void sendCommands();
#ifdef FEATURE_SERIAL
void updateSwapFrontBack();
#endif
bool loadProfileSettings();
bool saveProfileSettings();
void updateAccumulators();
void readPotis();
float wattToAmpere(float watt);
float wattToMotorCurrent(float watt);
uint8_t time_to_percent(espchrono::milliseconds32 repeat, espchrono::milliseconds32 riseTime, espchrono::milliseconds32 fullTime, size_t numLeds, bool invert);
std::string local_clock_string();
int16_t map_analog_stick(uint16_t middle, uint16_t start, uint16_t end, uint16_t raw);

inline uint32_t CRGB_TO_UINT32(CRGB crgb)
{
    return crgb.raw[0]<<16 | crgb.raw[1]<<8 | crgb.raw[2];
}

inline CRGB UINT32_TO_CRGB(uint32_t color)
{
    CRGB crgb;

    crgb.raw[0]=color>>16 & 0xFF;
    crgb.raw[1]=color>>8 & 0xFF;
    crgb.raw[2]=color & 0xFF;

    return crgb;
}

std::string get_wifi_security_string(wifi_auth_mode_t authMode);
float float_map(float x, float in_min, float in_max, float out_min, float out_max);
bool is_valid_timestamp(espchrono::utc_clock::time_point timestamp);
