#include "statusdisplay.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <espwifistack.h>
#include <fmt/core.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "drivingstatistics.h"
#include "modes/defaultmode.h"
#include "newsettings.h"
#include "screens/batteryinfodisplay.h"
#include "screens/mainmenu.h"
#include "screens/metersdisplay.h"
#include "screens/speedinfodisplay.h"
#include "taskmanager.h"
#include "udpcloud.h"

namespace bobby {

using namespace std::chrono_literals;
using namespace espgui;

namespace {
constexpr const char * const TAG = "STATUS";
} // namespace

void StatusDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.drawString("gas", 0, 0, espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelRawGas.start(tft);
    m_labelGas.start(tft);
    m_progressBarGas.start(tft);
    tft.drawString("brems", 0, 15, espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelRawBrems.start(tft);
    m_labelBrems.start(tft);
    m_progressBarBrems.start(tft);

    m_batterypercent.start(tft);
    m_watthoursleft.start(tft);
    m_kilometersleft.start(tft);

    m_frontStatus.start(tft);
    m_backStatus.start(tft);

    tft.drawString("WiFi:", 0, bottomLines[0], espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelWifiStatus.start(tft);
    tft.drawString("Lim0:", 173, bottomLines[0], espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelLimit0.start(tft);
    tft.drawString("IP:", 0, bottomLines[1], espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelIpAddress.start(tft);
    m_labelSignal.start(tft);
    tft.drawString("Lim1:", 173, bottomLines[1], espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelLimit1.start(tft);
    tft.drawString("Perf:", 0, bottomLines[2], espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelPerformance.start(tft);
    m_labelFreeMem.start(tft);
    tft.drawString("Mode:", 125, bottomLines[2], espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelMode.start(tft);
    tft.drawString("Name:", 0, bottomLines[3], espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelName.start(tft);
    m_labelProfile.start(tft);
}

void StatusDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    {
        const auto now = espchrono::millis_clock::now();
        if (now - lastRedraw < 50ms)
            return;
        lastRedraw = now;
    }

    {
        static bool handbremse_fill_with_black;
        if (configs.handbremse.enable.value() && configs.handbremse.visualize.value() && handbremse::angezogen)
        {
            tft.fillRect(0, tft.height()-6, tft.width(), 6, espgui::TFT_RED);
            handbremse_fill_with_black = true;
        }
        else if (configs.handbremse.enable.value() && configs.handbremse.visualize.value() && handbremse::stateWish == handbremse::StateWish::brake)
        {
            tft.fillRect(0, tft.height()-6, tft.width(), 6, espgui::TFT_YELLOW);
            handbremse_fill_with_black = true;
        }
        else if (handbremse_fill_with_black)
        {
            handbremse_fill_with_black = false;
            tft.fillRect(0, tft.height()-6, tft.width(), 6, espgui::TFT_BLACK);
        }
    }

    if(configs.feature.ledstrip.isEnabled.value())
    {
        static bool blink_fill_with_black;
        if (configs.ledstrip.enableVisualizeBlink.value() && (espchrono::utc_clock::now().time_since_epoch() % 750ms < 375ms) && (blinkAnimation > 0))
        {
            if (BLINK_LEFT_EXPR)
                tft.fillRect(0, 0, tft.width() / 2, 6, espgui::TFT_YELLOW);
            if (BLINK_RIGHT_EXPR)
                tft.fillRect(tft.width() / 2, 0, tft.width() / 2, 6, espgui::TFT_YELLOW);

            blink_fill_with_black = true;
        }
        else if (blink_fill_with_black)
        {
            blink_fill_with_black = false;
            tft.fillRect(0, 0, tft.width(), 6, espgui::TFT_BLACK);
            tft.drawString("gas", 0, 0, espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
            m_labelRawGas.start(tft);
            m_labelGas.start(tft);
            m_progressBarGas.start(tft);
        }
    }

    m_labelRawGas.redraw(tft, raw_gas ? std::to_string(*raw_gas) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelGas.redraw(tft, gas ? fmt::format("{:.2f}", *gas) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_progressBarGas.redraw(tft, gas ? *gas : 0);
    m_labelRawBrems.redraw(tft, raw_brems ? std::to_string(*raw_brems) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelBrems.redraw(tft, brems ? fmt::format("{:.2f}", *brems) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_progressBarBrems.redraw(tft, brems ? *brems : 0);

    m_batterypercent.redraw(tft, getBatteryPercentageString(), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_watthoursleft.redraw(tft, getBatteryRemainingWattHoursString(), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_kilometersleft.redraw(tft, getRemainingRangeString(), getEfficiencyClassColor(), espgui::TFT_BLACK, 2);

    m_frontStatus.redraw(tft, controllers.front);
    m_backStatus.redraw(tft, controllers.back);

    if (configs.feature.udpcloud.isEnabled.value())
    {
        if(configs.udpCloudSettings.udpCloudEnabled.value() && configs.udpCloudSettings.enableCloudDebug.value())
        {
            tft.fillRect(125, 258, 8, 8, (visualSendUdpPacket) ? espgui::TFT_DARKGREY : espgui::TFT_BLACK);
        }
//      else // is not needed because of redraw
//      {
//          tft.fillRect(125, 258, 8, 8, espgui::TFT_BLACK);
//      }
    }

    const auto staStatus = wifi_stack::get_sta_status();
    if (staStatus == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        if (const auto result = wifi_stack::get_sta_ap_info(); result)
        {
            m_labelWifiStatus.redraw(tft, std::string_view{reinterpret_cast<const char*>(result->ssid)}, espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
            m_labelSignal.redraw(tft, fmt::format("{}dB", result->rssi), (result->rssi < -80) ? espgui::TFT_ORANGE : espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
        }
        else
        {
            ESP_LOGW(TAG, "get_sta_ap_info() failed with %.*s", result.error().size(), result.error().data());
            goto showStaStatus;
        }
    }
    else
    {
showStaStatus:
        m_labelWifiStatus.redraw(tft, wifi_stack::toString(staStatus), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
        m_labelSignal.clear(tft, espgui::TFT_BLACK);
    }

    m_labelLimit0.redraw(tft, fmt::format("{}A", controllers.front.command.left.iMotMax), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);

    if (staStatus == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        if (const auto result = wifi_stack::get_ip_info(wifi_stack::esp_netifs[ESP_IF_WIFI_STA]); result)
            m_labelIpAddress.redraw(tft, wifi_stack::toString(result->ip), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
        else
        {
            ESP_LOGW(TAG, "get_ip_info() failed with %.*s", result.error().size(), result.error().data());
            goto clearIp;
        }
    }
    else
    {
clearIp:
        m_labelIpAddress.clear(tft, espgui::TFT_BLACK);
    }

    m_labelLimit1.redraw(tft, fmt::format("{}A", controllers.front.command.left.iDcMax), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);

    m_labelPerformance.redraw(tft, std::to_string(drivingModeTask.callCount()), (drivingModeTask.callCount() < 35) ? espgui::TFT_ORANGE : espgui::TFT_WHITE, espgui::TFT_BLACK, 2);

    {
        const auto freeMem = heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
        m_labelFreeMem.redraw(tft, fmt::format("{}K", freeMem/1000), (freeMem < 70000) ? espgui::TFT_ORANGE : espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    }

    m_labelMode.redraw(tft, currentMode->displayName(), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    m_labelName.redraw(tft, configs.wifiApName.value(), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
    const auto profile = settingsPersister.currentlyOpenProfileIndex();
    m_labelProfile.redraw(tft, profile ? std::to_string(*profile) : "-", espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
}

void StatusDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Right:
        pushScreen<MainMenu>();
        break;
    case Button::Up:
        if (simplified)
            return;
        switchScreen<SpeedInfoDisplay>();
        break;
    case Button::Down:
        if (simplified)
            return;
        switchScreen<MetersDisplay>();
        break;
    default:;
    }
}

StatusDisplay::BoardStatus::BoardStatus(int y) :
    m_y{y},
    m_labelLeftPwm{65, y}, // 80, 22
    m_labelRightPwm{155, y}, // 80, 22
    m_labelVoltage{30, y+25}, // 85, 22
    m_labelTemperature{150, y+25}, // 85, 22
    m_leftMotor{y+50},
    m_rightMotor{y+75}
{}

void StatusDisplay::BoardStatus::start(espgui::TftInterface &tft)
{
    tft.drawString("pwm:", 0, m_y, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_labelLeftPwm.start(tft);
    m_labelRightPwm.start(tft);
    m_initialRedraw = true;
}

void StatusDisplay::BoardStatus::redraw(espgui::TftInterface &tft, const Controller &controller)
{
    m_labelLeftPwm.redraw(tft, std::to_string(controller.command.left.pwm), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_labelRightPwm.redraw(tft, std::to_string(controller.command.right.pwm), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    if (controller.feedbackValid != m_lastFeedbackValid || m_initialRedraw)
    {
        tft.fillRect(0, m_y+25, tft.width(), 75, espgui::TFT_BLACK);

        if (controller.feedbackValid)
        {
            tft.drawString("U=", 0, m_y+25, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
            m_labelVoltage.start(tft);
            tft.drawString("T=", 120, m_y+25, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
            m_labelTemperature.start(tft);
            tft.drawString("l:", 0, m_y+50, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
            m_leftMotor.start(tft);
            tft.drawString("r:", 0, m_y+75, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
            m_rightMotor.start(tft);
        }
        else
        {
            tft.drawString("No data!", 60, m_y+50, espgui::TFT_RED, espgui::TFT_BLACK, 4);

            tft.setSwapBytes(true);
            tft.pushImage(10, m_y+40, bobbyicons::alert.WIDTH, bobbyicons::alert.HEIGHT, bobbyicons::alert.buffer);
            tft.setSwapBytes(false);
        }

        m_lastFeedbackValid = controller.feedbackValid;
        m_initialRedraw = false;
    }

    if (controller.feedbackValid)
    {
        m_labelVoltage.redraw(tft, fmt::format("{:.2f}V", controller.getCalibratedVoltage()), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
        m_labelTemperature.redraw(tft, fmt::format("{:.2f}C", fixBoardTemp(controller.feedback.boardTemp)), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
        m_leftMotor.redraw(tft, controller.feedback.left);
        m_rightMotor.redraw(tft, controller.feedback.right);
    }
}

StatusDisplay::BoardStatus::MotorStatus::MotorStatus(int y) :
    m_labelError{18, y}, // 18, 22,
    m_labelCurrent{40, y}, // 85, 22
    m_labelSpeed{135, y}, // 75, 22
    m_labelHallSensors{210, y} // 30, 15
{}

void StatusDisplay::BoardStatus::MotorStatus::start(espgui::TftInterface &tft)
{
    m_labelError.start(tft);
    m_labelCurrent.start(tft);
    m_labelSpeed.start(tft);
    m_labelHallSensors.start(tft);
}

void StatusDisplay::BoardStatus::MotorStatus::redraw(espgui::TftInterface &tft, const bobbycar::protocol::serial::MotorFeedback &motor)
{
    m_labelError.redraw(tft, std::to_string(motor.error), motor.error ? espgui::TFT_RED : espgui::TFT_GREEN, espgui::TFT_BLACK, 4);

    m_labelCurrent.redraw(tft, fmt::format("{:.2f}A", fixCurrent(motor.dcLink)), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_labelSpeed.redraw(tft, fmt::format("{:.2f}", convertToKmh(motor.speed)), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    m_labelHallSensors.redraw(tft, hallString(motor), espgui::TFT_WHITE, espgui::TFT_BLACK, 2);
}
} // namespace bobby
