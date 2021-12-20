#include "utils.h"

// local includes
#include "globals.h"

using namespace std::chrono_literals;

bool currentlyReverseBeeping{};
bool reverseBeepToggle;
espchrono::millis_clock::time_point lastReverseBeepToggle;

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
    return fmt::format("Unknown ControlType({})", std::to_underlying(value));
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
    return fmt::format("Unknown ControlMode({})", std::to_underlying(value));
}

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

    if (settings.buzzer.reverseBeep)
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
                    controller.command.buzzer = {.freq=settings.buzzer.reverseBeepFreq0, .pattern=0};
            }
            else
                for (auto &controller : controllers)
                    controller.command.buzzer = {};

            currentlyReverseBeeping = shouldBeep;
        }
        else if (shouldBeep && espchrono::millis_clock::now() - lastReverseBeepToggle >= std::chrono::milliseconds{reverseBeepToggle?settings.buzzer.reverseBeepDuration0:settings.buzzer.reverseBeepDuration1})
        {
            reverseBeepToggle = !reverseBeepToggle;

            for (auto &controller : controllers)
                controller.command.buzzer = {.freq=uint8_t(reverseBeepToggle?settings.buzzer.reverseBeepFreq0:settings.buzzer.reverseBeepFreq1), .pattern=0};

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
    {
        controllers.front.command.left.pwm = -controllers.front.command.left.pwm;
        controllers.front.command.left.nCruiseMotTgt = -controllers.front.command.left.nCruiseMotTgt;
    }
    if (settings.controllerHardware.invertFrontRight)
    {
        controllers.front.command.right.pwm = -controllers.front.command.right.pwm;
        controllers.front.command.right.nCruiseMotTgt = -controllers.front.command.right.nCruiseMotTgt;
    }
    if (settings.controllerHardware.invertBackLeft)
    {
        controllers.back.command.left.pwm = -controllers.back.command.left.pwm;
        controllers.back.command.left.nCruiseMotTgt = -controllers.back.command.left.nCruiseMotTgt;
    }
    if (settings.controllerHardware.invertBackRight)
    {
        controllers.back.command.right.pwm = -controllers.back.command.right.pwm;
        controllers.back.command.right.nCruiseMotTgt = -controllers.back.command.right.nCruiseMotTgt;
    }
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

#ifdef FEATURE_SERIAL
void updateSwapFrontBack()
{
    controllers.front.serial = settings.controllerHardware.swapFrontBack ? Serial2 : Serial1;
    controllers.back.serial = settings.controllerHardware.swapFrontBack ? Serial1 : Serial2;
}
#endif

bool loadSettings()
{
    bool result{true};
    if (!settingsPersister.load(settings))
        result = false;
    if (!settingsPersister.load(stringSettings))
        result = false;
    return result;
}

bool saveSettings()
{
    if (simplified) return true;
    bool result{true};
    if (!settingsPersister.save(settings))
        result = false;
    if (!settingsPersister.save(stringSettings))
        result = false;
    return result;
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

float wattToAmpere(float watt) {
    float voltage = std::max(controllers.front.feedback.batVoltage, controllers.back.feedback.batVoltage);
    if (voltage > 50) voltage = 50;
    if (voltage < 30) voltage = 30;
    return watt / voltage;
}

float wattToMotorCurrent(float watt)
{
    return wattToAmpere(watt) / 4;
}

uint8_t time_to_percent(espchrono::milliseconds32 repeat, espchrono::milliseconds32 riseTime, espchrono::milliseconds32 fullTime, size_t numLeds, bool invert)
{
    const auto now = espchrono::millis_clock::now().time_since_epoch() % repeat;
    if (now <= riseTime)
    {
        if (invert)
            return numLeds - ((now*numLeds) / riseTime);
        else
            return (now*numLeds) / riseTime;
    }
    else if (now < riseTime + fullTime)
        return invert ? 0 : numLeds;
    else
        return invert ? numLeds : 0;
}

void time_set_now(espchrono::utc_clock::time_point now)
{
    using namespace espchrono;
    // ESP_LOGI("BOBBY", "%s (%lld)%s", toString(toDateTime(now)).c_str(), std::chrono::floor<std::chrono::seconds>(now.time_since_epoch()).count(), time_valid(now) ? "":" (probably invalid)");
    const auto seconds = std::chrono::floor<std::chrono::seconds>(now.time_since_epoch());

    timeval ts {
        .tv_sec = (long int)seconds.count(),
        .tv_usec = (long int)std::chrono::floor<std::chrono::microseconds>(now.time_since_epoch() - seconds).count()
    };
    timezone tz {
        .tz_minuteswest = 0,
        .tz_dsttime = 0
    };
    settimeofday(&ts, &tz);
}

std::string local_clock_string()
{
    const auto dt = espchrono::toDateTime(espchrono::utc_clock::now() + settings.timeSettings.timezoneOffset);
    return fmt::format("{:02d}:{:02d}:{:02d}", dt.hour, dt.minute, dt.second);
}
