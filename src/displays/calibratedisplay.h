#pragma once

#include <array>

#include <WString.h>

#include "demodisplay.h"
#include "actions/switchscreenaction.h"
#include "globals.h"
#include "texts.h"
#include "widgets/label.h"
#include "widgets/progressbar.h"
#include "modes/ignoreinputmode.h"

namespace {
class StatusDisplay;
class BoardcomputerHardwareSettingsMenu;
}

namespace {
class CalibrateDisplay : public DemoDisplay
{
    using Base = DemoDisplay;

public:
    CalibrateDisplay() = default;
    CalibrateDisplay(bool bootup);

    void start() override;
    void initScreen() override;
    void redraw() override;
    void stop() override;

    void back() override;

    void triggered() override;

private:
    const bool m_bootup{false};
    bool saved{false};
    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{
      0,
#ifdef GLUMP_CONTROLLER
      ControlType::FieldOrientedControl,
      ControlMode::Torque
#endif
    };

    std::array<Label, 4> m_labels {{
        Label{25, 50}, // 100, 23
        Label{25, 75}, // 100, 23
        Label{25, 100}, // 100, 23
        Label{25, 125} // 100, 23
    }};

    std::array<ProgressBar, 2> m_progressBars {{
        ProgressBar{20, 200, 200, 10, 0, 1000},
        ProgressBar{20, 230, 200, 10, 0, 1000}
    }};

    Label savedLabel{25, 50};
};

CalibrateDisplay::CalibrateDisplay(bool bootup) :
    m_bootup{bootup}
{
}

void CalibrateDisplay::start()
{
    m_oldMode = currentMode;
    currentMode = &m_mode;
}

void CalibrateDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString(TEXT_CALIBRATE, 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    if (!saved) {
      for (auto &label : m_labels)
        label.start();

      for (auto &progressBar : m_progressBars)
        progressBar.start();
    }

    savedLabel.start();
}

void CalibrateDisplay::redraw()
{
    if (saved) {
        savedLabel.redraw(String{"Saved"});
        return;
    }

    m_labels[0].redraw(String{gas});
    m_labels[1].redraw(String{raw_gas});

    m_labels[2].redraw(String{brems});
    m_labels[3].redraw(String{raw_brems});

    m_progressBars[0].redraw(gas);
    m_progressBars[1].redraw(brems);
}

void CalibrateDisplay::stop()
{
    if (currentMode == &m_mode)
        currentMode = m_oldMode;
}

void CalibrateDisplay::back()
{
    if (!m_bootup)
#ifdef GLUMP_CONTROLLER
        switchScreen<BoardcomputerHardwareSettingsMenu>();
#endif
#ifdef VESC_CONTROLLER
        switchScreen<StatusDisplay>();
#endif
}

void CalibrateDisplay::triggered()
{
#ifdef GLUMP_CONTROLLER
    if (m_bootup)
        switchScreen<StatusDisplay>();
    else
        switchScreen<BoardcomputerHardwareSettingsMenu>();
#endif
#ifdef VESC_CONTROLLER
    if (saved) {
        switchScreen<BoardcomputerHardwareSettingsMenu>();
        return;
    }

    settings.boardcomputerHardware.gasMax = raw_gas * 0.9;
    settings.boardcomputerHardware.bremsMax = raw_brems * 0.9;
    saveSettings();

    saved = true;

    initScreen();
#endif
}
}
