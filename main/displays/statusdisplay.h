#pragma once

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "display.h"
#include "actions/switchscreenaction.h"
#include "modeinterface.h"
#include "globals.h"
#include "utils.h"
#include "widgets/label.h"
#include "widgets/progressbar.h"
#include "icons/alert.h"

// forward declares
namespace {
class MainMenu;
class BmsDisplay;
class MetersDisplay;
}

namespace {
class StatusDisplay : public Display, public ConfirmActionInterface<SwitchScreenAction<MainMenu>>, public DummyBack
{
public:
    void initScreen() override;
    void redraw() override;

    void rotate(int offset) override;

private:
    class BoardStatus
    {
    public:
        BoardStatus(int y) :
            m_y{y},
            m_labelLeftPwm{65, y}, // 80, 22
            m_labelRightPwm{155, y}, // 80, 22
            m_labelVoltage{30, y+25}, // 85, 22
            m_labelTemperature{150, y+25}, // 85, 22
            m_leftMotor{y+50},
            m_rightMotor{y+75}
        {}

        void start();
        void redraw(const Controller &controller);

    private:
        class MotorStatus
        {
        public:
            MotorStatus(int y) :
                m_labelError{18, y}, // 18, 22,
                m_labelCurrent{40, y}, // 85, 22
                m_labelSpeed{135, y}, // 75, 22
                m_labelHallSensors{210, y} // 30, 15
            {}

            void start();
            void redraw(const bobbycar::protocol::serial::MotorFeedback &motor);

        private:
            Label m_labelError;
            Label m_labelCurrent;
            Label m_labelSpeed;
            Label m_labelHallSensors;
        };

        const int m_y;

        bool m_initialRedraw;
        bool m_lastFeedbackValid;

        Label m_labelLeftPwm;
        Label m_labelRightPwm;

        Label m_labelVoltage;
        Label m_labelTemperature;

        MotorStatus m_leftMotor;
        MotorStatus m_rightMotor;
    };

    Label m_labelRawGas{45, 0}; // 40, 15
    Label m_labelGas{90, 0}; // 60, 15
    ProgressBar m_progressBarGas{150, 0, 90, 15, 0, 1000};

    Label m_labelRawBrems{45, 15}; // 40, 15
    Label m_labelBrems{90, 15}; // 60, 15
    ProgressBar m_progressBarBrems{150, 15, 90, 15, 0, 1000};

    BoardStatus m_frontStatus{42};
    BoardStatus m_backStatus{142};

    Label m_labelWifiStatus{35, bottomLines[0]}; // 120, 15
    Label m_labelLimit0{205, bottomLines[0]}; // 35, 15
    Label m_labelIpAddress{25, bottomLines[1]}; // 130, 15
    Label m_labelLimit1{205, bottomLines[1]}; // 35, 15
    Label m_labelPerformance{85, bottomLines[2]}; // 40, 15
    Label m_labelMode{165, bottomLines[2]}; // 75, 15
    Label m_labelName{40, bottomLines[3]}; // 40, 15
    Label m_labelProfile{205, bottomLines[3]}; // 35, 15

    static const constexpr int bottomLines[4] { 251, 266, 281, 296 };
};

void StatusDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
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

    m_frontStatus.start();
    m_backStatus.start();

    tft.setTextFont(2);
    tft.drawString("WiFi:", 0, bottomLines[0]);
    m_labelWifiStatus.start();
    tft.drawString("Limit0:", 160, bottomLines[0]);
    m_labelLimit0.start();
    tft.drawString("IP:", 0, bottomLines[1]);
    m_labelIpAddress.start();
    tft.drawString("Limit1:", 160, bottomLines[1]);
    m_labelLimit1.start();
    tft.drawString("Performance:", 0, bottomLines[2]);
    m_labelPerformance.start();
    tft.drawString("Mode:", 125, bottomLines[2]);
    m_labelMode.start();
    tft.drawString("Name:", 0, bottomLines[3]);
    m_labelName.start();
    m_labelProfile.start();

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void StatusDisplay::redraw()
{
    tft.setTextFont(2);
    m_labelRawGas.redraw(raw_gas ? std::to_string(*raw_gas) : "?");
    m_labelGas.redraw(gas ? fmt::format("{:.2f}", *gas) : "?");
    m_progressBarGas.redraw(gas ? *gas : 0);
    m_labelRawBrems.redraw(raw_brems ? std::to_string(*raw_brems) : "?");
    m_labelBrems.redraw(brems ? fmt::format("{:.2f}", *brems) : "?");
    m_progressBarBrems.redraw(brems ? *brems : 0);

    m_frontStatus.redraw(controllers.front);
    m_backStatus.redraw(controllers.back);

    tft.setTextFont(2);
    m_labelWifiStatus.redraw(wifi_stack::toString(wifi_stack::get_sta_status()));
    m_labelLimit0.redraw(std::to_string(controllers.front.command.left.iMotMax) + "A");
    if (const auto result = wifi_stack::get_ip_info(TCPIP_ADAPTER_IF_STA))
        m_labelIpAddress.redraw(wifi_stack::toString(result->ip));
    else
        m_labelIpAddress.clear();
    m_labelLimit1.redraw(std::to_string(controllers.front.command.left.iDcMax) + "A");
    m_labelPerformance.redraw(std::to_string(performance.last));
    m_labelMode.redraw(currentMode->displayName());
    m_labelName.redraw(deviceName);
    const auto profile = settingsPersister.currentlyOpenProfileIndex();
    m_labelProfile.redraw(profile ? std::to_string(*profile) : "-");
}

void StatusDisplay::rotate(int offset)
{
    if (offset < 0)
#ifdef FEATURE_BMS
        switchScreen<BmsDisplay>();
#else
        switchScreen<MetersDisplay>();
#endif
    else if (offset > 0)
        switchScreen<MetersDisplay>();
}

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

            tft.setSwapBytes(!settings.boardcomputerHardware.swapScreenBytes);
            tft.pushImage(10, m_y+40, icons::alert.WIDTH, icons::alert.HEIGHT, icons::alert.buffer);
            tft.setSwapBytes(settings.boardcomputerHardware.swapScreenBytes);
        }

        m_lastFeedbackValid = controller.feedbackValid;
        m_initialRedraw = false;
    }

    if (controller.feedbackValid)
    {
        m_labelVoltage.redraw(fmt::format("{:.2f}V", fixBatVoltage(controller.feedback.batVoltage)));
        m_labelTemperature.redraw(fmt::format("{:.2f}C", fixBoardTemp(controller.feedback.boardTemp)));
        m_leftMotor.redraw(controller.feedback.left);
        m_rightMotor.redraw(controller.feedback.right);
    }
}

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
}
