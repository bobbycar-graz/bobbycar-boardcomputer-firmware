#pragma once

// 3rdparty lib includes
#include <delayedconstruction.h>
#include <espchrono.h>
#include <tftinterface.h>
#include <widgets/progressbar.h>

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"

namespace bobby {

class SetupInformationDisplay : public virtual BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;
public:
    void initScreen(espgui::TftInterface &tft) override;
    void start() override;
    void update() override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

    [[nodiscard]] std::string text() const override;
private:
    espchrono::millis_clock::time_point m_menu_opened_timestamp;
    cpputils::DelayedConstruction<espgui::ProgressBar> m_init_text_progressbar;
};
} // namespace bobby
