#pragma once

// 3rdparty lib includes
#include <widgets/label.h>
#include <widgets/progressbar.h>

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"

namespace bobby {

class UpdateDisplay : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    std::string text() const override;

    void buttonPressed(espgui::Button button) override;

private:
    espgui::Label m_statusLabel{120, 75};
    espgui::Label m_progressLabel{120, 100};
    espgui::Label m_totalLabel{120, 125};
    espgui::Label m_messageLabel{20, 150};

    espgui::ProgressBar m_progressBar{20, 200, 200, 10, 0, 100};

    espgui::Label m_newVersionLabel{20, 275};
};
} // namespace bobby
