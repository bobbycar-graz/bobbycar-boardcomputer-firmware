#include "statusdisplay.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <espwifistack.h>
#include <tftinstance.h>

// local includes
#include "displays/menus/mainmenu.h"
#ifdef FEATURE_BMS
#include "displays/bmsdisplay.h"
#else
#include "displays/metersdisplay.h"
#endif
#include "drivingstatistics.h"

using namespace espgui;

namespace {
constexpr const char * const TAG = "STATUS";
} // namespace

void StatusDisplay::initScreen()
{
    Base::initScreen();

    tft.setTextFont(2);
    tft.setTextColor(TFT_WHITE);

    tft.drawString("gas", 0, 0);
    m_labelRawGas.start();
    m_labelGas.start();
    m_progressBarGas.start();
    tft.drawString("brems", 0, 15);
    m_labelRawBrems.start();
    m_labelBrems.start();
    m_progressBarBrems.start();

    m_batterypercent.start();
    m_watthoursleft.start();
    m_kilometersleft.start();

    m_frontStatus.start();
    m_backStatus.start();

    tft.setTextFont(2);
    tft.drawString("WiFi:", 0, bottomLines[0]);
    m_labelWifiStatus.start();
    tft.drawString("Lim0:", 173, bottomLines[0]);
    m_labelLimit0.start();
    tft.drawString("IP:", 0, bottomLines[1]);
    m_labelIpAddress.start();
    m_labelSignal.start();
    tft.drawString("Lim1:", 173, bottomLines[1]);
    m_labelLimit1.start();
    tft.drawString("Perf:", 0, bottomLines[2]);
    m_labelPerformance.start();
    m_labelFreeMem.start();
    tft.drawString("Mode:", 125, bottomLines[2]);
    m_labelMode.start();
    tft.drawString("Name:", 0, bottomLines[3]);
    m_labelName.start();
    m_labelProfile.start();

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void StatusDisplay::redraw()
{
    Base::redraw();

    tft.setTextFont(2);
    m_labelRawGas.redraw(raw_gas ? std::to_string(*raw_gas) : "?");
    m_labelGas.redraw(gas ? fmt::format("{:.2f}", *gas) : "?");
    m_progressBarGas.redraw(gas ? *gas : 0);
    m_labelRawBrems.redraw(raw_brems ? std::to_string(*raw_brems) : "?");
    m_labelBrems.redraw(brems ? fmt::format("{:.2f}", *brems) : "?");
    m_progressBarBrems.redraw(brems ? *brems : 0);

    m_batterypercent.redraw(getBatteryPercentageString());
    m_watthoursleft.redraw(getBatteryRemainingWattHoursString());
    const uint16_t efficiencyColor = getEfficiencyClassColor();
//    if (abs(avgSpeedKmh) > 2)
    {
        tft.setTextColor(efficiencyColor, TFT_BLACK);
    }
    m_kilometersleft.redraw(getRemainingRangeString());
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    m_frontStatus.redraw(controllers.front);
    m_backStatus.redraw(controllers.back);

    tft.setTextFont(2);

    const auto staStatus = wifi_stack::get_sta_status();
    if (staStatus == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        if (const auto result = wifi_stack::get_sta_ap_info(); result)
        {
            m_labelWifiStatus.redraw(std::string_view{reinterpret_cast<const char*>(result->ssid)});
            tft.setTextColor(result->rssi < -80 ? TFT_ORANGE : TFT_WHITE, TFT_BLACK);
            m_labelSignal.redraw(fmt::format("{}dB", result->rssi));
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        else
        {
            ESP_LOGW("BOBBY", "get_sta_ap_info() failed with %.*s", result.error().size(), result.error().data());
            goto showStaStatus;
        }
    }
    else
    {
showStaStatus:
        m_labelWifiStatus.redraw(wifi_stack::toString(staStatus));
        m_labelSignal.clear();
    }

    m_labelLimit0.redraw(fmt::format("{}A", controllers.front.command.left.iMotMax));

    if (staStatus == wifi_stack::WiFiStaStatus::CONNECTED)
    {
        if (const auto result = wifi_stack::get_ip_info(TCPIP_ADAPTER_IF_STA); result)
            m_labelIpAddress.redraw(wifi_stack::toString(result->ip));
        else
        {
            ESP_LOGW("BOBBY", "get_ip_info() failed with %.*s", result.error().size(), result.error().data());
            goto clearIp;
        }
    }
    else
    {
clearIp:
        m_labelIpAddress.clear();
    }

    m_labelLimit1.redraw(fmt::format("{}A", controllers.front.command.left.iDcMax));

    tft.setTextColor(performance.last < 35 ? TFT_ORANGE : TFT_WHITE, TFT_BLACK);
    m_labelPerformance.redraw(std::to_string(performance.last));
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    {
        const auto freeMem = heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
        tft.setTextColor(freeMem < 70000 ? TFT_ORANGE : TFT_WHITE, TFT_BLACK);
        m_labelFreeMem.redraw(fmt::format("{}K", freeMem/1000));
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }

    m_labelMode.redraw(currentMode->displayName());
    m_labelName.redraw(deviceName);
    const auto profile = settingsPersister.currentlyOpenProfileIndex();
    m_labelProfile.redraw(profile ? std::to_string(*profile) : "-");
}

void StatusDisplay::confirm()
{
    switchScreen<MainMenu>();
}

void StatusDisplay::back()
{

}

void StatusDisplay::rotate(int offset)
{
    Base::rotate(offset);

    if (offset < 0)
#ifdef FEATURE_BMS
        switchScreen<BmsDisplay>();
#else
        switchScreen<MetersDisplay>();
#endif
    else if (offset > 0)
        switchScreen<MetersDisplay>();
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

void StatusDisplay::BoardStatus::start()
{
    tft.setTextFont(4);
    tft.drawString("pwm:", 0, m_y);
    m_labelLeftPwm.start();
    m_labelRightPwm.start();
    m_initialRedraw = true;
}

void StatusDisplay::BoardStatus::redraw(const Controller &controller)
{
    tft.setTextFont(4);

    m_labelLeftPwm.redraw(std::to_string(controller.command.left.pwm));
    m_labelRightPwm.redraw(std::to_string(controller.command.right.pwm));

    if (controller.feedbackValid != m_lastFeedbackValid || m_initialRedraw)
    {
        tft.fillRect(0, m_y+25, tft.width(), 75, TFT_BLACK);

        if (controller.feedbackValid)
        {
            tft.setTextColor(TFT_WHITE);

            tft.drawString("U=", 0, m_y+25, 4);
            m_labelVoltage.start();
            tft.drawString("T=", 120, m_y+25, 4);
            m_labelTemperature.start();
            tft.drawString("l:", 0, m_y+50, 4);
            m_leftMotor.start();
            tft.drawString("r:", 0, m_y+75, 4);
            m_rightMotor.start();

            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        else
        {
            tft.setTextColor(TFT_RED);
            tft.drawString("No data!", 60, m_y+50, 4);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);

            tft.setSwapBytes(true);
            tft.pushImage(10, m_y+40, bobbyicons::alert.WIDTH, bobbyicons::alert.HEIGHT, bobbyicons::alert.buffer);
            tft.setSwapBytes(false);
        }

        m_lastFeedbackValid = controller.feedbackValid;
        m_initialRedraw = false;
    }

    if (controller.feedbackValid)
    {
        m_labelVoltage.redraw(fmt::format("{:.2f}V", controller.getCalibratedVoltage()));
        m_labelTemperature.redraw(fmt::format("{:.2f}C", fixBoardTemp(controller.feedback.boardTemp)));
        m_leftMotor.redraw(controller.feedback.left);
        m_rightMotor.redraw(controller.feedback.right);
    }
}

StatusDisplay::BoardStatus::MotorStatus::MotorStatus(int y) :
    m_labelError{18, y}, // 18, 22,
    m_labelCurrent{40, y}, // 85, 22
    m_labelSpeed{135, y}, // 75, 22
    m_labelHallSensors{210, y} // 30, 15
{}

void StatusDisplay::BoardStatus::MotorStatus::start()
{
    m_labelError.start();
    m_labelCurrent.start();
    m_labelSpeed.start();
    m_labelHallSensors.start();
}

void StatusDisplay::BoardStatus::MotorStatus::redraw(const bobbycar::protocol::serial::MotorFeedback &motor)
{
    tft.setTextFont(4);
    tft.setTextColor(motor.error?TFT_RED:TFT_GREEN, TFT_BLACK);
    m_labelError.redraw(std::to_string(motor.error));
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    m_labelCurrent.redraw(fmt::format("{:.2f}A", fixCurrent(motor.dcLink)));
    m_labelSpeed.redraw(fmt::format("{:.2f}", convertToKmh(motor.speed)));

    tft.setTextFont(2);
    m_labelHallSensors.redraw(hallString(motor));

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}
