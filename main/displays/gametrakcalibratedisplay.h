#pragma once

#include <array>
#include <string>

#include "display.h"
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
#ifdef FEATURE_GAMETRAK
class GametrakCalibrateDisplay : public Display, public ConfirmActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>, public BackActionInterface<SwitchScreenAction<BoardcomputerHardwareSettingsMenu>>
{
public:
    void initScreen() override;
    void redraw() override;

private:
    std::array<Label, 6> m_labels {{
        Label{25, 50}, // 100, 23
        Label{120, 50}, // 100, 23
        Label{25, 75}, // 100, 23
        Label{120, 75}, // 100, 23
        Label{25, 100}, // 100, 23
        Label{120, 100} // 100, 23
    }};

    std::array<ProgressBar, 3> m_progressBars {{
        ProgressBar{20, 200, 200, 10, 0, 1000},
        ProgressBar{20, 230, 200, 10, 0, 1000},
        ProgressBar{20, 260, 200, 10, 0, 1000}
    }};
};

void GametrakCalibrateDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString(TEXT_GAMETRAKCALIBRATE, 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    for (auto &label : m_labels)
        label.start();

    for (auto &progressBar : m_progressBars)
        progressBar.start();
}

void GametrakCalibrateDisplay::redraw()
{
    m_labels[0].redraw(std::to_string(gametrakX));
    m_labels[1].redraw(std::to_string(raw_gametrakX));

    m_labels[2].redraw(std::to_string(gametrakY));
    m_labels[3].redraw(std::to_string(raw_gametrakY));

    m_labels[4].redraw(std::to_string(gametrakDist));
    m_labels[5].redraw(std::to_string(raw_gametrakDist));

    m_progressBars[0].redraw(gametrakX);
    m_progressBars[1].redraw(gametrakY);
    m_progressBars[2].redraw(gametrakDist);
}
#endif
}
