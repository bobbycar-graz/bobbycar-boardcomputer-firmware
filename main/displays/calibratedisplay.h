#pragma once

#include <array>
#include <string>

#include "display.h"
#include "actions/switchscreenaction.h"
#include "globals.h"
#include "utils.h"
#include "texts.h"
#include "widgets/label.h"
#include "widgets/progressbar.h"
#include "modes/ignoreinputmode.h"

namespace {
class StatusDisplay;
class BoardcomputerHardwareSettingsMenu;
}

namespace {
class CalibrateDisplay : public Display
{
public:
    CalibrateDisplay() = default;
    CalibrateDisplay(bool bootup);

    void start() override;
    void initScreen() override;
    void update() override;
    void redraw() override;
    void stop() override;

    void rotate(int offset) override;

    void back() override;

    void confirm() override;

private:
    void copyFromSettings();
    void copyToSettings();

    const bool m_bootup{false};
    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Torque};

    std::array<Label, 11> m_labels {{
        Label{25, 72}, // 100, 23
        Label{145, 72}, // 100, 23
        Label{25, 97}, // 100, 23
        Label{145, 97}, // 100, 23

        Label{25, 172}, // 100, 23
        Label{145, 172}, // 100, 23
        Label{25, 197}, // 100, 23
        Label{145, 197}, // 100, 23

        Label{25, 247}, // 190, 23

        Label{25, 277}, // 100, 23
        Label{145, 277}, // 100, 23
    }};

    std::array<ProgressBar, 2> m_progressBars {{
        ProgressBar{20, 129, 200, 10, 0, 1000},
        ProgressBar{20, 229, 200, 10, 0, 1000}
    }};

    enum Status {
        Begin,
        GasMin,
        GasMax,
        BremsMin,
        BremsMax,
        Confirm
    };

    int8_t m_selectedButton, m_renderedButton;

    Status m_status;
    int16_t m_gasMin, m_gasMax, m_bremsMin, m_bremsMax;
    std::optional<float> m_gas, m_brems;
};

CalibrateDisplay::CalibrateDisplay(bool bootup) :
    m_bootup{bootup}
{
}

void CalibrateDisplay::start()
{
    m_oldMode = currentMode;
    currentMode = &m_mode;
    m_selectedButton = 0;
    m_status = Status::Begin;
    copyFromSettings();
    m_gas = std::nullopt;
    m_brems = std::nullopt;
}

void CalibrateDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString(TEXT_CALIBRATE, 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString("gas:", 25, 47);
    tft.drawString("brems:", 25, 147);

    for (auto &label : m_labels)
        label.start();

    for (auto &progressBar : m_progressBars)
        progressBar.start();

    m_renderedButton = -1;
}

void CalibrateDisplay::update()
{
    if (raw_gas)
        m_gas = scaleBetween<float>(*raw_gas, m_gasMin, m_gasMax, 0., 1000.);
    else
        m_gas = std::nullopt;

    if (raw_brems)
        m_brems = scaleBetween<float>(*raw_brems, m_bremsMin, m_bremsMax, 0., 1000.);
    else
        m_brems = std::nullopt;
}

void CalibrateDisplay::redraw()
{
    m_labels[0].redraw(m_gas ? std::to_string(*m_gas) : "?");
    m_labels[1].redraw(raw_gas ? std::to_string(*raw_gas) : "?");
    if (m_status == Status::GasMin)
        tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[2].redraw(std::to_string(m_gasMin));
    if (m_status == Status::GasMin)
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    if (m_status == Status::GasMax)
        tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[3].redraw(std::to_string(m_gasMax));
    if (m_status == Status::GasMax)
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

    m_progressBars[0].redraw(m_gas ? *m_gas : 0);

    m_labels[4].redraw(m_brems ? std::to_string(*m_brems) : "?");
    m_labels[5].redraw(raw_brems ? std::to_string(*raw_brems) : "?");
    if (m_status == Status::BremsMin)
        tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[6].redraw(std::to_string(m_bremsMin));
    if (m_status == Status::BremsMin)
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    if (m_status == Status::BremsMax)
        tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[7].redraw(std::to_string(m_bremsMax));
    if (m_status == Status::BremsMax)
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

    m_progressBars[1].redraw(m_brems ? *m_brems : 0);

    m_labels[8].redraw([&](){
        switch (m_status)
        {
        case Status::Begin: return "Start calibrating?";
        case Status::GasMin: return "Release gas";
        case Status::GasMax: return "Press gas";
        case Status::BremsMin: return "Release brems";
        case Status::BremsMax: return "Press brems";
        case Status::Confirm: return "Verify";
        }
        __builtin_unreachable();
    }());

    {
        const auto failed = !m_gas || !m_brems || (m_status == Status::Confirm && (*m_gas > 100 || *m_brems > 100));
        const auto color = failed ? TFT_DARKGREY : TFT_WHITE;
        tft.setTextColor(color, TFT_BLACK);
        m_labels[9].redraw([&](){
            switch (m_status)
            {
            case Status::Begin: return "Yes";
            case Status::GasMin:
            case Status::GasMax:
            case Status::BremsMin:
            case Status::BremsMax: return "Next";
            case Status::Confirm: return "Save";
            }
            __builtin_unreachable();
        }());

        if (m_selectedButton != m_renderedButton && (m_selectedButton == 0 || m_renderedButton == 0))
            tft.drawRect(3, 275, 100, 27, m_selectedButton == 0 ? color : TFT_BLACK);
    }

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    m_labels[10].redraw([&](){
        switch (m_status)
        {
        case Status::Begin: return "No";
        case Status::GasMin:
        case Status::GasMax:
        case Status::BremsMin:
        case Status::BremsMax:
        case Status::Confirm: return "Abort";
        }
        __builtin_unreachable();
    }());

    if (m_selectedButton != m_renderedButton && (m_selectedButton == 1 || m_renderedButton == 1))
        tft.drawRect(123, 275, 100, 27, m_selectedButton == 1 ? TFT_WHITE : TFT_BLACK);

    m_renderedButton = m_selectedButton;
}

void CalibrateDisplay::stop()
{
    if (currentMode == &m_mode)
        currentMode = m_oldMode;
}

void CalibrateDisplay::rotate(int offset)
{
    m_selectedButton += offset;

    if (m_selectedButton < 0)
        m_selectedButton = 1;
    if (m_selectedButton > 1)
        m_selectedButton = 0;
}

void CalibrateDisplay::back()
{
    switch (m_status)
    {
    case Status::Begin:
        if (m_bootup)
            switchScreen<StatusDisplay>();
        else
            switchScreen<BoardcomputerHardwareSettingsMenu>();
        break;
    case Status::GasMin:
    case Status::GasMax:
    case Status::BremsMin:
    case Status::BremsMax:
    case Status::Confirm:
        m_selectedButton = 0;
        m_status = Status::Begin;
        copyFromSettings();
    }
}

void CalibrateDisplay::confirm()
{
    switch (m_selectedButton)
    {
    case 0: // left button pressed
        if (!raw_gas || !raw_brems || !m_gas || !m_brems)
            return;

        switch (m_status)
        {
        case Status::Begin:
            m_status = Status::GasMin;
            break;
        case Status::GasMin:
            m_gasMin = *raw_gas;
            m_status = Status::GasMax;
            break;
        case Status::GasMax:
            m_gasMax = *raw_gas;
            m_status = Status::BremsMin;
            {
                const auto dead = (m_gasMax - m_gasMin)/20;
                m_gasMin += dead;
                m_gasMax -= dead;
            }
            break;
        case Status::BremsMin:
            m_bremsMin = *raw_brems;
            m_status = Status::BremsMax;
            break;
        case Status::BremsMax:
            m_bremsMax = *raw_brems;
            m_status = Status::Confirm;
            {
                const auto dead = (m_bremsMax - m_bremsMin)/20;
                m_bremsMin += dead;
                m_bremsMax -= dead;
            }
            break;
        case Status::Confirm:
            if (*m_gas > 100 || *m_brems > 100)
                return;
            copyToSettings();
            saveSettings();
            if (m_bootup)
                switchScreen<StatusDisplay>();
            else
                switchScreen<BoardcomputerHardwareSettingsMenu>();
        }
        break;
    case 1: // right button pressed
        back();
    }
}

void CalibrateDisplay::copyFromSettings()
{
    m_gasMin = settings.boardcomputerHardware.gasMin;
    m_gasMax = settings.boardcomputerHardware.gasMax;
    m_bremsMin = settings.boardcomputerHardware.bremsMin;
    m_bremsMax = settings.boardcomputerHardware.bremsMax;
}

void CalibrateDisplay::copyToSettings()
{
    settings.boardcomputerHardware.gasMin = m_gasMin;
    settings.boardcomputerHardware.gasMax = m_gasMax;
    settings.boardcomputerHardware.bremsMin = m_bremsMin;
    settings.boardcomputerHardware.bremsMax = m_bremsMax;
}
}
