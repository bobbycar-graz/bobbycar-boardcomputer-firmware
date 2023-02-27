#include "utils.h"

// 3rdparty lib includes
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "globals.h"
#include "newsettings.h"

using namespace std::chrono_literals;

bool currentlyReverseBeeping{};
bool reverseBeepToggle;
espchrono::millis_clock::time_point lastReverseBeepToggle;

float convertToKmh(float val)
{
    return val /* / profileSettings.controllerHardware.numMagnetPoles */ / 60.f * configs.controllerHardware.wheelDiameter.value() / 1000.f * 3.14159265359f * 3.6f;
}

float convertFromKmh(float val)
{
    return val /* * profileSettings.controllerHardware.numMagnetPoles */ * 60.f / configs.controllerHardware.wheelDiameter.value() * 1000.f / 3.14159265359f / 3.6f;
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
    return -value/50.f;
}

float fixBoardTemp(int16_t value)
{
    return value/10.f;
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
        motor.iMotMax = profileSettings.limits.iMotMax;
        motor.iDcMax = profileSettings.limits.iDcMax;
        motor.nMotMax = profileSettings.limits.nMotMax;
        motor.fieldWeakMax = profileSettings.limits.fieldWeakMax;
        motor.phaseAdvMax = profileSettings.limits.phaseAdvMax;
    }

    if (configs.reverseBeep.value())
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
                    controller.command.buzzer = {.freq=configs.reverseBeepFreq0.value(), .pattern=0};
            }
            else
                for (auto &controller : controllers)
                    controller.command.buzzer = {};

            currentlyReverseBeeping = shouldBeep;
        }
        else if (shouldBeep && espchrono::millis_clock::now() - lastReverseBeepToggle >= std::chrono::milliseconds{reverseBeepToggle?configs.reverseBeepDuration0.value():configs.reverseBeepDuration1.value()})
        {
            reverseBeepToggle = !reverseBeepToggle;

            for (auto &controller : controllers)
                controller.command.buzzer = {.freq=uint8_t(reverseBeepToggle?configs.reverseBeepFreq0.value():configs.reverseBeepFreq1.value()), .pattern=0};

            lastReverseBeepToggle = espchrono::millis_clock::now();
        }
    }
    else if (currentlyReverseBeeping)
    {
        for (auto &controller : controllers)
            controller.command.buzzer = {};
        currentlyReverseBeeping = false;
    }

    controllers.front.command.left.enable = profileSettings.controllerHardware.enableFrontLeft;
    controllers.front.command.right.enable = profileSettings.controllerHardware.enableFrontRight;
    controllers.back.command.left.enable = profileSettings.controllerHardware.enableBackLeft;
    controllers.back.command.right.enable = profileSettings.controllerHardware.enableBackRight;

    if (profileSettings.controllerHardware.invertFrontLeft)
    {
        controllers.front.command.left.pwm = -controllers.front.command.left.pwm;
        controllers.front.command.left.nCruiseMotTgt = -controllers.front.command.left.nCruiseMotTgt;
    }
    if (profileSettings.controllerHardware.invertFrontRight)
    {
        controllers.front.command.right.pwm = -controllers.front.command.right.pwm;
        controllers.front.command.right.nCruiseMotTgt = -controllers.front.command.right.nCruiseMotTgt;
    }
    if (profileSettings.controllerHardware.invertBackLeft)
    {
        controllers.back.command.left.pwm = -controllers.back.command.left.pwm;
        controllers.back.command.left.nCruiseMotTgt = -controllers.back.command.left.nCruiseMotTgt;
    }
    if (profileSettings.controllerHardware.invertBackRight)
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
    controllers.front.serial = configs.controllerHardware.swapFrontBack.value ? Serial2 : Serial1;
    controllers.back.serial = configs.controllerHardware.swapFrontBack.value ? Serial1 : Serial2;
}
#endif

bool loadProfileSettings()
{
    bool result{true};
    if (!settingsPersister.load(profileSettings))
        result = false;
    return result;
}

bool saveProfileSettings()
{
    if (simplified) return true;
    bool result{true};
    if (!settingsPersister.save(profileSettings))
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

    // accel
    EVERY_N_MILLIS(100)
    {
        const auto now = espchrono::millis_clock::now();
        const auto delta = now - lastAvgSpeedKmhTs;
        const auto deltaSpeedKmh = avgSpeedKmh - lastAvgSpeedKmh;

        // ESP_LOGI("utils.cpp", "delta: %lli ms, deltaSpeedKmh: %f", delta.count() / 1000, deltaSpeedKmh);

        const auto deltaMilliseconds = delta.count() / 1000.f;

        const auto kmh_s = deltaSpeedKmh / deltaMilliseconds; // km/h / s
        const auto m_s2 = kmh_s * 1000.f / 3600.f; // m/s^2

        // ESP_LOGI("utils.cpp", "m_s2: %f", m_s2);

        avgAccel = avgAccel * 0.3f + m_s2 * 0.7f;

        lastAvgSpeedKmh = avgSpeedKmh;
        lastAvgSpeedKmhTs = espchrono::millis_clock::now();
    }
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

std::string local_clock_string()
{
#ifdef CONFIG_ESPCHRONO_SUPPORT_DEFAULT_TIMEZONE
    const auto now = espchrono::local_clock::now();
#else // mir egal ob die lokalzeit richtig is
    const auto now = espchrono::utc_clock::now() + configs.timezoneOffset.value();
#endif
    const auto dt = espchrono::toDateTime(now);
    return fmt::format("{:02d}:{:02d}:{:02d}", dt.hour, dt.minute, dt.second);
}

int16_t map_analog_stick(uint16_t middle, uint16_t start, uint16_t end, uint16_t deadband, uint16_t raw)
{
    if (abs(raw - middle) < configs.deadband.value())
    {
        return 0;
    }

    if (start < end)
    {
        if (raw < middle)
        {
            raw += configs.deadband.value();
            start += configs.deadband.value();
            const auto return_val = cpputils::mapValueClamped<float>(raw, start, middle, -1000, 0);
            return return_val;
        }
        else
        {
            raw -= configs.deadband.value();
            end -= configs.deadband.value();
            const auto return_val = cpputils::mapValueClamped<float>(raw, middle, end, 0, 1000);
            return return_val;
        }
    }
    else
    {
        if (raw < middle)
        {
            raw += configs.deadband.value();
            end  += configs.deadband.value();
            const auto return_val = cpputils::mapValueClamped<float>(raw, end, middle, 1000, 0);
            return return_val;
        }
        else
        {
            raw -= configs.deadband.value();
            start -= configs.deadband.value();
            const auto return_val = cpputils::mapValueClamped<float>(raw, middle, start, 0, -1000);
            return return_val;
        }
    }
}

std::string get_wifi_security_string(wifi_auth_mode_t authMode)
{ // use for qr code
    switch (authMode)
    {
        case WIFI_AUTH_OPEN:
            return "";
        case WIFI_AUTH_WEP:
            return "WEP";
        case WIFI_AUTH_WPA_PSK:
        case WIFI_AUTH_WPA2_PSK:
        case WIFI_AUTH_WPA_WPA2_PSK:
            return "WPA";
        default:
            return "";
    }
}

float float_map(float x, float in_min, float in_max, float out_min, float out_max) {
    const float dividend = out_max - out_min;
    const float divisor = in_max - in_min;
    const float delta = x - in_min;

    return (delta * dividend + (divisor / 2.f)) / divisor + out_min;
}

bool is_valid_timestamp(espchrono::utc_clock::time_point timestamp)
{
    using namespace date;
    return timestamp.time_since_epoch() > sys_seconds{sys_days{1_d/January/2000}}.time_since_epoch();
}

std::string toString(esp_chip_model_t esp_chip_model)
{
    switch (esp_chip_model)
    {
    case CHIP_ESP32:
        return "ESP32";
    case CHIP_ESP32S2:
        return "ESP32S2";
    case CHIP_ESP32S3:
        return "ESP32S3";
    case CHIP_ESP32C3:
        return "ESP32C3";
    case CHIP_ESP32H4:
        return "ESP32H4";
    case CHIP_ESP32C2:
        return "ESP32C2";
    case CHIP_ESP32C6:
        return "ESP32C6";
    default:
        return "invalid";
    }
}

std::optional<SetupStep> checkIfInCalibration()
{
    if (!configs.boardcomputerHardware.setupFinished.value())
    {
        return SetupStep::INFORMATION;
    }
    else if (
        configs.dpadMappingLeft.value() == INPUT_MAPPING_NONE ||
        configs.dpadMappingRight.value() == INPUT_MAPPING_NONE ||
        configs.dpadMappingUp.value() == INPUT_MAPPING_NONE ||
        configs.dpadMappingDown.value() == INPUT_MAPPING_NONE
    )
    {
        return SetupStep::BASIC_BUTTONS;
    }
    else if (!gas || !brems || *gas > 200.f || *brems > 200.f)
    {
        return SetupStep::CALIBRATE_POTIS;
    }
    return std::nullopt;
}

void drawLargeText(espgui::TftInterface &tft, const std::string&& text)
{
    using namespace espgui;

    const auto topMargin = 50;
    const uint8_t leftMargin = 8;
    const auto rightMargin = leftMargin;
    const auto bottomMargin = leftMargin;

    int x = leftMargin + 5;
    int y = topMargin + 5;

    for (char c : text)
    {
        if (c == '\n' || x > tft.width() - rightMargin - 10)
        {
            x = leftMargin + 5;
            y += tft.fontHeight(2);
        }

        if (c != '\n')
        {
            const int16_t addedWidth = tft.drawChar(tft.decodeUTF8(c), x, y, espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
            x += addedWidth;
        }

        if (y >= tft.height() - bottomMargin)
            break;
    }
}
