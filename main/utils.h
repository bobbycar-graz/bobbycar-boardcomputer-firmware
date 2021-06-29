#pragma once

#include <algorithm>
#include <utility>
#include <string>

#include <driver/twai.h>

#ifdef FEATURE_ARDUINOOTA
#include <ArduinoOTA.h>
#endif
#ifdef FEATURE_SERIAL
#include <HardwareSerial.h>
#endif

#include <espchrono.h>

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

namespace {
bool currentlyReverseBeeping;
bool reverseBeepToggle;
espchrono::millis_clock::time_point lastReverseBeepToggle;

template<typename ...T>
class makeComponent : public T...
{};

template <typename T1, typename T2, typename ...T3>
class makeComponentArgs : public T1, public T2, public T3...
{
public:
    template<typename ...T>
    makeComponentArgs(T&& ...args) :
        T2{std::forward<T>(args)...}
    {
    }
};

template<typename T>
T scaleBetween(T x, T in_min, T in_max, T out_min, T out_max) {
    if (x < std::min(in_min, in_max))
        x = std::min(in_min, in_max);
    else if (x > std::max(in_min, in_max))
        x = std::max(in_min, in_max);

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
};

float convertToKmh(float val)
{
    return val /* / settings.controllerHardware.numMagnetPoles */ / 60.f * settings.controllerHardware.wheelDiameter / 1000.f * 3.14159265359f * 3.6f;
}

float convertFromKmh(float val)
{
    return val /* * settings.controllerHardware.numMagnetPoles */ * 60.f / settings.controllerHardware.wheelDiameter * 1000.f / 3.14159265359f / 3.6f;
}

float convertToInch(float val)
{
    return val / 25.4f;
}

float convertFromInch(float val)
{
    return val * 25.4f;
}

float fixCurrent(int16_t value)
{
    return -value/50.;
}

float fixBatVoltage(int16_t value)
{
    return value/100.;
}

float fixBoardTemp(int16_t value)
{
    return value/10.;
}

std::string hallString(const bobbycar::protocol::serial::MotorFeedback &motor)
{
    return std::string{} + (motor.hallA ? '1' : '0') + (motor.hallB ? '1' : '0') + (motor.hallC ? '1' : '0');
}

std::string to_string(bobbycar::protocol::ControlType value)
{
    switch (value)
    {
    using namespace bobbycar::protocol;
    case ControlType::Commutation: return "Commutation";
    case ControlType::Sinusoidal: return "Sinusoidal";
    case ControlType::FieldOrientedControl: return "FieldOrientedControl";
    }
    return "Unknown ControlType(" + std::to_string(int(value)) + ')';
}

std::string to_string(bobbycar::protocol::ControlMode value)
{
    switch (value)
    {
    using namespace bobbycar::protocol;
    case ControlMode::OpenMode: return "OpenMode";
    case ControlMode::Voltage: return "Voltage";
    case ControlMode::Speed: return "Speed";
    case ControlMode::Torque: return "Torque";
    }
    return "Unknown ControlMode(" + std::to_string(int(value)) + ')';
}

#ifdef FEATURE_ARDUINOOTA
std::string to_string(ota_error_t value)
{
    switch (value)
    {
    case OTA_AUTH_ERROR: return "OTA_AUTH_ERROR";
    case OTA_BEGIN_ERROR: return "OTA_BEGIN_ERROR";
    case OTA_CONNECT_ERROR: return "OTA_CONNECT_ERROR";
    case OTA_RECEIVE_ERROR: return "OTA_RECEIVE_ERROR";
    case OTA_END_ERROR: return "OTA_END_ERROR";
    }

    return "Unknown ota_error_t(" + std::to_string(int(value)) + ')';
}
#endif

std::array<std::reference_wrapper<bobbycar::protocol::serial::MotorState>, 2> motorsInController(Controller &controller)
{
    return {std::ref(controller.command.left), std::ref(controller.command.right)};
}

std::array<std::reference_wrapper<const bobbycar::protocol::serial::MotorState>, 2> motorsInController(const Controller &controller)
{
    return {std::ref(controller.command.left), std::ref(controller.command.right)};
}

std::array<std::reference_wrapper<bobbycar::protocol::serial::MotorFeedback>, 2> motorFeedbacksInController(Controller &controller)
{
    return {std::ref(controller.feedback.left), std::ref(controller.feedback.right)};
}

std::array<std::reference_wrapper<const bobbycar::protocol::serial::MotorFeedback>, 2> motorFeedbacksInController(const Controller &controller)
{
    return {std::ref(controller.feedback.left), std::ref(controller.feedback.right)};
}

std::array<std::reference_wrapper<bobbycar::protocol::serial::MotorState>, 4> motors()
{
    return {
        std::ref(controllers.front.command.left), std::ref(controllers.front.command.right),
        std::ref(controllers.back.command.left), std::ref(controllers.back.command.right)
    };
}

void fixCommonParams()
{
    for (bobbycar::protocol::serial::MotorState &motor : motors())
    {
        motor.iMotMax = settings.limits.iMotMax;
        motor.iDcMax = settings.limits.iDcMax;
        motor.nMotMax = settings.limits.nMotMax;
        motor.fieldWeakMax = settings.limits.fieldWeakMax;
        motor.phaseAdvMax = settings.limits.phaseAdvMax;
    }

    if (settings.reverseBeep)
    {
        const auto x = motors();
        const auto shouldBeep = std::all_of(std::begin(x), std::end(x), [](const bobbycar::protocol::serial::MotorState &motor){ return motor.pwm < 0; });

        if (shouldBeep != currentlyReverseBeeping)
        {
            if (shouldBeep)
            {
                reverseBeepToggle = true;
                lastReverseBeepToggle = espchrono::millis_clock::now();
                for (auto &controller : controllers)
                    controller.command.buzzer = {.freq=settings.reverseBeepFreq0, .pattern=0};
            }
            else
                for (auto &controller : controllers)
                    controller.command.buzzer = {};

            currentlyReverseBeeping = shouldBeep;
        }
        else if (shouldBeep && espchrono::millis_clock::now() - lastReverseBeepToggle >= std::chrono::milliseconds{reverseBeepToggle?settings.reverseBeepDuration0:settings.reverseBeepDuration1})
        {
            reverseBeepToggle = !reverseBeepToggle;

            for (auto &controller : controllers)
                controller.command.buzzer = {.freq=uint8_t(reverseBeepToggle?settings.reverseBeepFreq0:settings.reverseBeepFreq1), .pattern=0};

            lastReverseBeepToggle = espchrono::millis_clock::now();
        }
    }
    else if (currentlyReverseBeeping)
    {
        for (auto &controller : controllers)
            controller.command.buzzer = {};
        currentlyReverseBeeping = false;
    }

    controllers.front.command.left.enable = settings.controllerHardware.enableFrontLeft;
    controllers.front.command.right.enable = settings.controllerHardware.enableFrontRight;
    controllers.back.command.left.enable = settings.controllerHardware.enableBackLeft;
    controllers.back.command.right.enable = settings.controllerHardware.enableBackRight;

    if (settings.controllerHardware.invertFrontLeft)
        controllers.front.command.left.pwm = -controllers.front.command.left.pwm;
    if (settings.controllerHardware.invertFrontRight)
        controllers.front.command.right.pwm = -controllers.front.command.right.pwm;
    if (settings.controllerHardware.invertBackLeft)
        controllers.back.command.left.pwm = -controllers.back.command.left.pwm;
    if (settings.controllerHardware.invertBackRight)
        controllers.back.command.right.pwm = -controllers.back.command.right.pwm;
}

void sendCommands()
{
#ifdef FEATURE_SERIAL
    using namespace bobbycar::protocol::serial;
    for (Controller &controller : controllers)
    {
        controller.command.start = Command::VALID_HEADER;
        controller.command.checksum = calculateChecksum(controller.command);
        controller.serial.get().write((uint8_t *) &controller.command, sizeof(controller.command));
    }
#endif
#ifdef FEATURE_CAN
    can::sendCanCommands();
#endif
}

template<typename T, typename... Args>
void switchScreen(Args&&... args);

#ifdef FEATURE_SERIAL
void updateSwapFrontBack()
{
    controllers.front.serial = settings.controllerHardware.swapFrontBack ? Serial2 : Serial1;
    controllers.back.serial = settings.controllerHardware.swapFrontBack ? Serial1 : Serial2;
}
#endif

void loadSettings()
{
    settingsPersister.load(settings);
}

void saveSettings()
{
    settingsPersister.save(settings);
}

void updateAccumulators()
{
    avgSpeed = 0.f;
    sumCurrent = 0.f;
    uint8_t count{0};

    for (const Controller &controller : controllers)
    {
        if (!controller.feedbackValid)
            continue;

        avgSpeed +=
                controller.feedback.left.speed * (controller.invertLeft ? -1 : 1) +
                controller.feedback.right.speed * (controller.invertRight ? -1 : 1);

        sumCurrent +=
                controller.feedback.left.dcLink +
                controller.feedback.right.dcLink;

        count +=2;
    }

    if (count)
        avgSpeed /= count;

    sumCurrent = fixCurrent(sumCurrent);

    avgSpeedKmh = convertToKmh(avgSpeed);
}

void readPotis()
{    
    [[maybe_unused]]
    constexpr auto sampleMultipleTimes = [](int pin){
        analogRead(pin);
        double sum{};
        const auto sampleCount = settings.boardcomputerHardware.sampleCount;
        for (int i = 0; i < sampleCount; i++)
            sum += analogRead(pin);
        return sum / sampleCount;
    };

    raw_gas = std::nullopt;
    raw_brems = std::nullopt;

#ifdef FEATURE_CAN
    const auto now = espchrono::millis_clock::now();

    if (can::can_gas)
    {
        if (now - can::last_can_gas < 100ms)
            raw_gas = *can::can_gas;
        else
            can::can_gas = std::nullopt;
    }

    if (can::can_brems)
    {
        if (now - can::last_can_brems < 100ms)
            raw_brems = *can::can_brems;
        else
            can::can_brems = std::nullopt;
    }
#endif

#ifdef FEATURE_ADC_IN
    if (!raw_gas)
        raw_gas = sampleMultipleTimes(PINS_GAS);
    if (!raw_brems)
        raw_brems = sampleMultipleTimes(PINS_BREMS);
#endif

    if (raw_gas)
        gas = scaleBetween<float>(*raw_gas, settings.boardcomputerHardware.gasMin, settings.boardcomputerHardware.gasMax, 0., 1000.);
    else
        gas = std::nullopt;
    if (raw_brems)
        brems = scaleBetween<float>(*raw_brems, settings.boardcomputerHardware.bremsMin, settings.boardcomputerHardware.bremsMax, 0., 1000.);
    else
        brems = std::nullopt;

#ifdef FEATURE_GAMETRAK
    raw_gametrakX = sampleMultipleTimes(PINS_GAMETRAKX);
    gametrakX = scaleBetween<float>(raw_gametrakX, settings.boardcomputerHardware.gametrakXMin, settings.boardcomputerHardware.gametrakXMax, 0., 1000.);

    raw_gametrakY = sampleMultipleTimes(PINS_GAMETRAKY);
    gametrakY = scaleBetween<float>(raw_gametrakY, settings.boardcomputerHardware.gametrakYMin, settings.boardcomputerHardware.gametrakYMax, 0., 1000.);

    raw_gametrakDist = sampleMultipleTimes(PINS_GAMETRAKDIST);
    gametrakDist = scaleBetween<float>(raw_gametrakDist, settings.boardcomputerHardware.gametrakDistMin, settings.boardcomputerHardware.gametrakDistMax, 0., 1000.);
#endif
}
}
