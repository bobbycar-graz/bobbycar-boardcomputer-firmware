#pragma once

// esp-idf includes
#include <esp_heap_caps.h>

// 3rdparty lib includes
#include <display.h>
#include <actions/switchscreenaction.h>
#include <widgets/label.h>
#include <widgets/progressbar.h>
#include <espchrono.h>

// local includes
#include "modeinterface.h"
#include "globals.h"
#include "utils.h"
#include "icons/alert.h"
#include "battery.h"

class StatusDisplay :
    public espgui::Display
{
    using Base = espgui::Display;

public:
    void initScreen() override;
    void redraw() override;

    void confirm() override;
    void back() override;
    void rotate(int offset) override;

private:
    class BoardStatus
    {
    public:
        BoardStatus(int y);

        void start();
        void redraw(const Controller &controller);

    private:
        class MotorStatus
        {
        public:
            MotorStatus(int y);

            void start();
            void redraw(const bobbycar::protocol::serial::MotorFeedback &motor);

        private:
            espgui::Label m_labelError;
            espgui::Label m_labelCurrent;
            espgui::Label m_labelSpeed;
            espgui::Label m_labelHallSensors;
        };

        const int m_y;

        bool m_initialRedraw;
        bool m_lastFeedbackValid;

        espgui::Label m_labelLeftPwm;
        espgui::Label m_labelRightPwm;

        espgui::Label m_labelVoltage;
        espgui::Label m_labelTemperature;

        MotorStatus m_leftMotor;
        MotorStatus m_rightMotor;
    };

    espchrono::millis_clock::time_point lastRedraw;

    espgui::Label m_labelRawGas{45, 0}; // 40, 15
    espgui::Label m_labelGas{90, 0}; // 60, 15
    espgui::ProgressBar m_progressBarGas{150, 0, 90, 15, 0, 1000};

    espgui::Label m_labelRawBrems{45, 15}; // 40, 15
    espgui::Label m_labelBrems{90, 15}; // 60, 15
    espgui::ProgressBar m_progressBarBrems{150, 15, 90, 15, 0, 1000};

    espgui::Label m_batterypercent{0, 30};
    espgui::Label m_watthoursleft{107, 30};
    espgui::Label m_kilometersleft{176, 30};

    BoardStatus m_frontStatus{45};
    BoardStatus m_backStatus{145};

    espgui::Label m_labelWifiStatus{35, bottomLines[0]}; // 120, 15
    espgui::Label m_labelLimit0{205, bottomLines[0]}; // 35, 15
    espgui::Label m_labelIpAddress{25, bottomLines[1]}; // 130, 15
    espgui::Label m_labelSignal{125, bottomLines[1]}; // 130, 15
    espgui::Label m_labelLimit1{205, bottomLines[1]}; // 35, 15
    espgui::Label m_labelPerformance{40, bottomLines[2]}; // 40, 15
    espgui::Label m_labelFreeMem{70, bottomLines[2]}; // 40, 15
    espgui::Label m_labelMode{165, bottomLines[2]}; // 75, 15
    espgui::Label m_labelName{40, bottomLines[3]}; // 40, 15
    espgui::Label m_labelProfile{205, bottomLines[3]}; // 35, 15

    static const constexpr int bottomLines[4] { 251, 266, 281, 296 };
};
