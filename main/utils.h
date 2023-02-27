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

#ifdef FEATURE_DPAD
#include "dpad.h"
#endif
#ifdef FEATURE_DPAD_3WIRESW
#include "dpad3wire.h"
#endif
#ifdef FEATURE_DPAD_5WIRESW
#include "dpad5wire.h"
#endif
#ifdef FEATURE_DPAD_5WIRESW_2OUT
#include "dpad5wire_2out.h"
#endif
#ifdef FEATURE_DPAD_6WIRESW
#include "dpad6wire.h"
#endif
#ifdef DPAD_BOARDCOMPUTER_V2
#include "dpad_boardcomputer_v2.h"
#endif
#include "setup.h"

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
int16_t map_analog_stick(uint16_t middle, uint16_t start, uint16_t end, uint16_t deadband, uint16_t raw);

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

std::string toString(esp_chip_model_t esp_chip_model);
std::optional<SetupStep> checkIfInCalibration();
void drawLargeText(espgui::TftInterface &tft, const std::string&& text);

namespace bobbydpad {
#ifdef FEATURE_DPAD
static constexpr const auto ButtonCount = dpad::BUTTON_COUNT;
static constexpr const auto &dpad_init = dpad::init;
static constexpr const auto &dpad_update = dpad::update;
static constexpr const char * const dpad_name = "dpad";
#endif
#ifdef FEATURE_DPAD_3WIRESW
static constexpr const auto ButtonCount = dpad3wire::BUTTON_COUNT;
static constexpr const auto &dpad_init = dpad3wire::init;
static constexpr const auto &dpad_update = dpad3wire::update;
static constexpr const char * const dpad_name = "dpad3wire";
#endif
#ifdef FEATURE_DPAD_5WIRESW
static constexpr const auto ButtonCount = dpad5wire::BUTTON_COUNT;
static constexpr const auto &dpad_init = dpad5wire::init;
static constexpr const auto &dpad_update = dpad5wire::update;
static constexpr const char * const dpad_name = "dpad5wire";
#endif
#ifdef FEATURE_DPAD_5WIRESW_2OUT
static constexpr const auto ButtonCount = dpad5wire_2out::BUTTON_COUNT;
static constexpr const auto &dpad_init = dpad5wire_2out::init;
static constexpr const auto &dpad_update = dpad5wire_2out::update;
static constexpr const char * const dpad_name = "dpad5wire_2out";
#endif
#ifdef FEATURE_DPAD_6WIRESW
static constexpr const auto ButtonCount = dpad6wire::BUTTON_COUNT;
static constexpr const auto &dpad_init = dpad6wire::init;
static constexpr const auto &dpad_update = dpad6wire::update;
static constexpr const char * const dpad_name = "dpad6wire";
#endif
#ifdef DPAD_BOARDCOMPUTER_V2
static constexpr const auto ButtonCount = dpad_boardcomputer_v2::BUTTON_COUNT;
static constexpr const auto &dpad_init = dpad_boardcomputer_v2::init;
static constexpr const auto &dpad_update = dpad_boardcomputer_v2::update;
static constexpr const char * const dpad_name = "dpad_boardcomputer_v2";
#endif
#if !defined(FEATURE_DPAD) && !defined(FEATURE_DPAD_3WIRESW) && !defined(FEATURE_DPAD_5WIRESW) && !defined(FEATURE_DPAD_5WIRESW_2OUT) && !defined(FEATURE_DPAD_6WIRESW) && !defined(DPAD_BOARDCOMPUTER_V2)
static constexpr const auto ButtonCount = 0;
static constexpr const auto &dpad_init = nullptr;
static constexpr const auto &dpad_update = nullptr;
static constexpr const char * const dpad_name = "";
#endif
} // namespace bobbydpad
