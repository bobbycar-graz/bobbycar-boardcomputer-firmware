#include "can.h"
#ifdef FEATURE_CAN

// system includes
#include <cstring>
#include <optional>

// esp-idf
#include <driver/twai.h>
#include <driver/gpio.h>
#include <esp_log.h>

// 3rdparty lib includes
#include <espchrono.h>
#include <tickchrono.h>
#include <screenmanager.h>

// local includes
#include "bobbycar-can.h"
#include "globals.h"
#include "newsettings.h"
#include "bobbybuttons.h"

using namespace std::chrono_literals;

namespace can {
uint32_t can_total_error_cnt;
namespace {
constexpr const char * const TAG = "BOBBYCAN";

bool tryParseCanInput();
} // namespace

std::optional<int16_t> can_gas, can_brems;
espchrono::millis_clock::time_point last_can_gas{}, last_can_brems{};

void initCan()
{
    ESP_LOGI(TAG, "called");

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config TWAI_TIMING_CONFIG_250KBITS();
    twai_filter_config_t f_config TWAI_FILTER_CONFIG_ACCEPT_ALL();
//    {
//                           //
//        .acceptance_code = 0b00000000000,
//        .acceptance_mask = 0b00001111111,
//        .single_filter = true
//    };

    if (const auto result = twai_driver_install(&g_config, &t_config, &f_config); result == ESP_OK)
    {
        ESP_LOGI(TAG, "twai_driver_install() succeeded");
    }
    else
    {
        ESP_LOGE(TAG, "twai_driver_install() failed with %s", esp_err_to_name(result));
        return;
    }

    if (const auto result = twai_start(); result == ESP_OK)
    {
        ESP_LOGI(TAG, "twai_start() succeeded");
    }
    else
    {
        ESP_LOGE(TAG, "twai_start() failed with %s", esp_err_to_name(result));

        if (const auto result = twai_driver_uninstall(); result == ESP_OK)
        {
            ESP_LOGI(TAG, "twai_driver_uninstall() succeeded");
        }
        else
        {
            ESP_LOGE(TAG, "twai_driver_uninstall() failed with %s", esp_err_to_name(result));
        }

        return;
    }
}

void updateCan()
{
    for (int i = 0; i < 4; i++)
        if (!tryParseCanInput())
            break;
}

namespace {

template<bool isBack>
bool parseMotorControllerCanMessage(const twai_message_t &message, Controller &controller)
{
    switch (message.identifier)
    {
    using namespace bobbycar::protocol::can;
    case MotorController<isBack, false>::Feedback::DcLink:
        controller.feedback.left.dcLink = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::DcLink:
        controller.feedback.right.dcLink = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::Speed:
        controller.feedback.left.speed = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::Speed:
        controller.feedback.right.speed = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::Error:
        controller.feedback.left.error = *((int8_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::Error:
        controller.feedback.right.error = *((int8_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::Angle:
        controller.feedback.left.angle = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::Angle:
        controller.feedback.right.angle = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::DcPhaA:
        controller.feedback.left.dcPhaA = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::DcPhaA:
        controller.feedback.right.dcPhaA = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::DcPhaB:
        controller.feedback.left.dcPhaB = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::DcPhaB:
        controller.feedback.right.dcPhaB = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::DcPhaC:
        controller.feedback.left.dcPhaC = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::DcPhaC:
        controller.feedback.right.dcPhaC = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::Chops:
        controller.feedback.left.chops = *((uint16_t*)message.data);
        return true;
    case MotorController<isBack, true>::Feedback::Chops:
        controller.feedback.right.chops = *((uint16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::Hall:
        controller.feedback.left.hallA = *((uint8_t*)message.data) & 1;
        controller.feedback.left.hallB = *((uint8_t*)message.data) & 2;
        controller.feedback.left.hallC = *((uint8_t*)message.data) & 4;
        return true;
    case MotorController<isBack, true>::Feedback::Hall:
        controller.feedback.right.hallA = *((uint8_t*)message.data) & 1;
        controller.feedback.right.hallB = *((uint8_t*)message.data) & 2;
        controller.feedback.right.hallC = *((uint8_t*)message.data) & 4;
        return true;
    case MotorController<isBack, false>::Feedback::Voltage:
    case MotorController<isBack, true>::Feedback::Voltage:
        controller.feedback.batVoltage = *((int16_t*)message.data);
        return true;
    case MotorController<isBack, false>::Feedback::Temp:
    case MotorController<isBack, true>::Feedback::Temp:
        controller.feedback.boardTemp = *((int16_t*)message.data);
        return true;
    }

    return false;
}

bool parseBoardcomputerCanMessage(const twai_message_t &message)
{
    switch (message.identifier)
    {
    using namespace bobbycar::protocol::can;
    case Boardcomputer::Command::RawButtonPressed:
        if (espgui::currentDisplay)
            espgui::currentDisplay->rawButtonPressed(*((const uint8_t*)message.data));
        break;
    case Boardcomputer::Command::RawButtonReleased:
        if (espgui::currentDisplay)
            espgui::currentDisplay->rawButtonReleased(*((const uint8_t*)message.data));
        break;
    case Boardcomputer::Command::ButtonPressed:
        if (espgui::currentDisplay)
            espgui::currentDisplay->rawButtonPressed(espgui::Button(*((const uint8_t*)message.data)));
        break;
    case Boardcomputer::Command::ButtonReleased:
        if (espgui::currentDisplay)
            espgui::currentDisplay->rawButtonPressed(espgui::Button(*((const uint8_t*)message.data)));
        break;
    case Boardcomputer::Command::RawGas:
        can_gas = *((int16_t*)message.data);
        last_can_gas = espchrono::millis_clock::now();
        break;
    case Boardcomputer::Command::RawBrems:
        can_brems = *((int16_t*)message.data);
        last_can_brems = espchrono::millis_clock::now();
        break;
    }

    return false;
}

bool tryParseCanInput()
{
    twai_message_t message;
    const auto timeout = std::chrono::ceil<espcpputils::ticks>(espchrono::milliseconds32{configs.controllerHardware.canReceiveTimeout.value}).count();
    if (const auto result = twai_receive(&message, timeout); result != ESP_OK)
    {
        if (result != ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(TAG, "twai_receive() failed with %s", esp_err_to_name(result));
        }

        if (espchrono::millis_clock::now() - controllers.front.lastCanFeedback > 100ms)
            controllers.front.feedbackValid = false;

        if (espchrono::millis_clock::now() - controllers.back.lastCanFeedback > 100ms)
            controllers.back.feedbackValid = false;

        return false;
    }

    Controller &front = configs.controllerHardware.swapFrontBack.value ? controllers.back : controllers.front;
    Controller &back = configs.controllerHardware.swapFrontBack.value ? controllers.front : controllers.back;

    if (parseMotorControllerCanMessage<false>(message, front))
    {
        if (espchrono::millis_clock::now() - back.lastCanFeedback > 100ms)
            back.feedbackValid = false;

        front.lastCanFeedback = espchrono::millis_clock::now();
        front.feedbackValid = true;
        return true;
    }
    else
    {
        if (espchrono::millis_clock::now() - front.lastCanFeedback > 100ms)
            front.feedbackValid = false;
    }

    if (parseMotorControllerCanMessage<true>(message, back))
    {
        back.lastCanFeedback = espchrono::millis_clock::now();
        back.feedbackValid = true;
        return true;
    }
    else
    {
        if (espchrono::millis_clock::now() - back.lastCanFeedback > 100ms)
            back.feedbackValid = false;
    }

    if (parseBoardcomputerCanMessage(message))
        return true;

    ESP_LOGW(TAG, "Unknown CAN info received .identifier = %u", message.identifier);

    return true;
}

} // namespace

void sendCanCommands()
{
    static uint32_t can_sequential_error_cnt = 0;

    constexpr auto send = [](uint32_t addr, auto value){
        twai_message_t message;
        message.identifier = addr;
        message.flags = TWAI_MSG_FLAG_SS;
        message.data_length_code = sizeof(value);
        std::fill(std::begin(message.data), std::end(message.data), 0);
        std::memcpy(message.data, &value, sizeof(value));

        const auto timeout = std::chrono::ceil<espcpputils::ticks>(espchrono::milliseconds32{configs.controllerHardware.canTransmitTimeout.value}).count();

        const auto timestamp_before = espchrono::millis_clock::now();
        const auto result = twai_transmit(&message, timeout);
        const auto timestamp_after = espchrono::millis_clock::now();

        if (result == ESP_ERR_TIMEOUT)
        {
            ++can_sequential_error_cnt;
            ++can_total_error_cnt;

            ESP_LOGW(TAG, "twai_transmit() failed after %lldms with %s, seq err: %d, total err: %d",
                     (timestamp_after - timestamp_before).count(),
                     esp_err_to_name(result),
                     can_sequential_error_cnt,
                     can_total_error_cnt);
        }
        else if (result != ESP_OK)
        {
            ESP_LOGE(TAG, "ERROR: twai_transmit() failed after %lldms with %s",
                     (timestamp_after - timestamp_before).count(),
                     esp_err_to_name(result));
        }
        else
        {
            can_sequential_error_cnt = 0;
        }


        if (can_sequential_error_cnt > 3)
        {
            can_sequential_error_cnt = 0;
            if (configs.canResetOnError.value)
            {
                ESP_LOGW(TAG, "WARNING: Something isn't right, trying to restart can ic...");
                if (const auto err = twai_stop(); err != ESP_OK)
                {
                    ESP_LOGE(TAG, "ERROR: twai_stop() failed with %s", esp_err_to_name(err));
                }
                if (const auto err = twai_start(); err != ESP_OK)
                {
                    ESP_LOGE(TAG, "ERROR: twai_start() failed with %s", esp_err_to_name(err));
                }
            }
        }
        return result;
    };

    const bool swap = configs.controllerHardware.swapFrontBack.value;
    const Controller *front =
            (swap ? configs.controllerHardware.sendBackCanCmd.value : configs.controllerHardware.sendFrontCanCmd.value ) ?
            (swap ? &controllers.back : &controllers.front) :
            nullptr;
    const Controller *back =
            (swap ? configs.controllerHardware.sendFrontCanCmd.value : configs.controllerHardware.sendBackCanCmd.value ) ?
            (swap ? &controllers.front : &controllers.back) :
            nullptr;

    using namespace bobbycar::protocol::can;

#ifdef HAS_SIMPLIFIED
    SIMPLIFIED_PWM
#else
    if (front) send(MotorController<false, false>::Command::InpTgt, front->command.left.pwm);
    if (front) send(MotorController<false, true>::Command::InpTgt, front->command.right.pwm);
    if (back) send(MotorController<true, false>::Command::InpTgt, back->command.left.pwm);
    if (back) send(MotorController<true, true>::Command::InpTgt, back->command.right.pwm);
#endif

    uint16_t buttonLeds{};
    if (const auto index = settingsPersister.currentlyOpenProfileIndex())
        switch (*index)
        {
        case 0: buttonLeds |= Boardcomputer::ButtonProfile0; break;
        case 1: buttonLeds |= Boardcomputer::ButtonProfile1; break;
        case 2: buttonLeds |= Boardcomputer::ButtonProfile2; break;
        case 3: buttonLeds |= Boardcomputer::ButtonProfile3; break;
        }

    static struct {
        struct {
            uint8_t freq = 0;
            uint8_t pattern = 0;
        } front, back;
        uint16_t buttonLeds{};
    } lastValues;

    static int i{};

    if ((front && front->command.buzzer.freq    != lastValues.front.freq ) ||
        (front && front->command.buzzer.pattern != lastValues.front.pattern ) ||
        (back && back->command.buzzer.freq     != lastValues.back.freq) ||
        (back && back->command.buzzer.pattern  != lastValues.back.pattern))
        i = 10;
    else if (buttonLeds              != lastValues.buttonLeds)
        i = 12;

    switch (i++)
    {
    case 0:
        if (front) send(MotorController<false, false>::Command::Enable, front->command.left.enable);
        if (front) send(MotorController<false, true>::Command::Enable, front->command.right.enable);
        if (back) send(MotorController<true, false>::Command::Enable, back->command.left.enable);
        if (back) send(MotorController<true, true>::Command::Enable, back->command.right.enable);
        break;
    case 1:
#ifdef HAS_SIMPLIFIED
        SIMPLIFIED_CONTROLTYPE
#endif
        if (front) send(MotorController<false, false>::Command::CtrlTyp, front->command.left.ctrlTyp);
        if (front) send(MotorController<false, true>::Command::CtrlTyp, front->command.right.ctrlTyp);
        if (back) send(MotorController<true, false>::Command::CtrlTyp, back->command.left.ctrlTyp);
        if (back) send(MotorController<true, true>::Command::CtrlTyp, back->command.right.ctrlTyp);
        break;
    case 2:
#ifdef HAS_SIMPLIFIED
        SIMPLIFIED_CONTROLMODE
#endif
        if (front) send(MotorController<false, false>::Command::CtrlMod, front->command.left.ctrlMod);
        if (front) send(MotorController<false, true>::Command::CtrlMod, front->command.right.ctrlMod);
        if (back) send(MotorController<true, false>::Command::CtrlMod, back->command.left.ctrlMod);
        if (back) send(MotorController<true, true>::Command::CtrlMod, back->command.right.ctrlMod);
        handbremse::finishedMotorUpdate = true;
        break;
    case 3:
#ifdef HAS_SIMPLIFIED
        SIMPLIFIED_IMOTMAX
#endif
            if (front) send(MotorController<false, false>::Command::IMotMax, front->command.left.iMotMax);
            if (front) send(MotorController<false, true>::Command::IMotMax, front->command.right.iMotMax);
            if (back) send(MotorController<true, false>::Command::IMotMax, back->command.left.iMotMax);
            if (back) send(MotorController<true, true>::Command::IMotMax, back->command.right.iMotMax);
        break;
    case 4:
#ifdef HAS_SIMPLIFIED
        SIMPLIFIED_IDCMAX
#endif
            if (front) send(MotorController<false, false>::Command::IDcMax, front->command.left.iDcMax);
            if (front) send(MotorController<false, true>::Command::IDcMax, front->command.right.iDcMax);
            if (back) send(MotorController<true, false>::Command::IDcMax, back->command.left.iDcMax);
            if (back) send(MotorController<true, true>::Command::IDcMax, back->command.right.iDcMax);
        break;
    case 5:
#ifdef HAS_SIMPLIFIED
        SIMPLIFIED_NMOTMAX
#endif
            if (front) send(MotorController<false, false>::Command::NMotMax, front->command.left.nMotMax);
            if (front) send(MotorController<false, true>::Command::NMotMax, front->command.right.nMotMax);
            if (back) send(MotorController<true, false>::Command::NMotMax, back->command.left.nMotMax);
            if (back) send(MotorController<true, true>::Command::NMotMax, back->command.right.nMotMax);
        break;
    case 6:
#ifdef HAS_SIMPLIFIED
        SIMPLIFIED_FIELDWEAKMAX
#endif
            if (front) send(MotorController<false, false>::Command::FieldWeakMax, front->command.left.fieldWeakMax);
            if (front) send(MotorController<false, true>::Command::FieldWeakMax, front->command.right.fieldWeakMax);
            if (back) send(MotorController<true, false>::Command::FieldWeakMax, back->command.left.fieldWeakMax);
            if (back) send(MotorController<true, true>::Command::FieldWeakMax, back->command.right.fieldWeakMax);
        break;
    case 7:
        if (front) send(MotorController<false, false>::Command::PhaseAdvMax, front->command.left.phaseAdvMax);
        if (front) send(MotorController<false, true>::Command::PhaseAdvMax, front->command.right.phaseAdvMax);
        if (back) send(MotorController<true, false>::Command::PhaseAdvMax, back->command.left.phaseAdvMax);
        if (back) send(MotorController<true, true>::Command::PhaseAdvMax, back->command.right.phaseAdvMax);
        break;
    case 8:
        if (front) send(MotorController<false, false>::Command::CruiseCtrlEna, front->command.left.cruiseCtrlEna);
        if (front) send(MotorController<false, true>::Command::CruiseCtrlEna, front->command.right.cruiseCtrlEna);
        if (back) send(MotorController<true, false>::Command::CruiseCtrlEna, back->command.left.cruiseCtrlEna);
        if (back) send(MotorController<true, true>::Command::CruiseCtrlEna, back->command.right.cruiseCtrlEna);
        break;
    case 9:
        if (front) send(MotorController<false, false>::Command::CruiseMotTgt, front->command.left.nCruiseMotTgt);
        if (front) send(MotorController<false, true>::Command::CruiseMotTgt, front->command.right.nCruiseMotTgt);
        if (back) send(MotorController<true, false>::Command::CruiseMotTgt, back->command.left.nCruiseMotTgt);
        if (back) send(MotorController<true, true>::Command::CruiseMotTgt, back->command.right.nCruiseMotTgt);
        break;
    case 10:
        if (front && send(MotorController<false, false>::Command::BuzzerFreq, front->command.buzzer.freq) == ESP_OK)
            lastValues.front.freq = front->command.buzzer.freq;
//        if (front && send(MotorController<false, true>::Command::BuzzerFreq, front->command.buzzer.freq) == ESP_OK)
//            lastValues.front.freq = front->command.buzzer.freq;
        if (back && send(MotorController<true, false>::Command::BuzzerFreq, back->command.buzzer.freq) == ESP_OK)
            lastValues.back.freq = back->command.buzzer.freq;
//        if (back && send(MotorController<true, true>::Command::BuzzerFreq, back->command.buzzer.freq) == ESP_OK)
//            lastValues.back.freq = back->command.buzzer.freq;
        if (front && send(MotorController<false, false>::Command::BuzzerPattern, front->command.buzzer.pattern) == ESP_OK)
            lastValues.front.pattern = front->command.buzzer.pattern;
//        if (front && send(MotorController<false, true>::Command::BuzzerPattern, front->command.buzzer.pattern) == ESP_OK)
//            lastValues.front.pattern = front->command.buzzer.pattern;
        if (back && send(MotorController<true, false>::Command::BuzzerPattern, back->command.buzzer.pattern) == ESP_OK)
            lastValues.back.pattern = back->command.buzzer.pattern;
//        if (back && send(MotorController<true, true>::Command::BuzzerPattern, back->command.buzzer.pattern) == ESP_OK)
//            lastValues.back.pattern = back->command.buzzer.pattern;
        break;
    case 11:
        if (front) send(MotorController<false, false>::Command::Led, front->command.led);
        //if (front) send(MotorController<false, true>::Command::Led, front->command.led);
        if (back) send(MotorController<true, false>::Command::Led, back->command.led);
        //if (back) send(MotorController<true, true>::Command::Led, back->command.led);
        if (front) send(MotorController<false, false>::Command::Poweroff, front->command.poweroff);
        //if (front) send(MotorController<false, true>::Command::Poweroff, front->command.poweroff);
        if (back) send(MotorController<true, false>::Command::Poweroff, back->command.poweroff);
        //if (back) send(MotorController<true, true>::Command::Poweroff, back->command.poweroff);
        break;
    case 12:
        if (send(Boardcomputer::Feedback::ButtonLeds, buttonLeds) == ESP_OK)
            lastValues.buttonLeds = buttonLeds;
    [[fallthrough]];
    default:
        i=0;
        break;
    }
}

} // namespace can
#endif
