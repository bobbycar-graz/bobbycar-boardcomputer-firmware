#pragma once

// 3rdparty lib includes
#include <espchrono.h>
#include <tftinstance.h>
#include <widgets/progressbar.h>

// local includes
#include "displays/bobbydisplaywithtitle.h"

class SetupInformationDisplay : public virtual BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;
public:
    void initScreen() override;
    void start() override;
    void update() override;

    void buttonPressed(espgui::Button button) override;

    [[nodiscard]] std::string text() const override;
private:
    espchrono::millis_clock::time_point m_menu_opened_timestamp;
    espgui::ProgressBar m_init_text_progressbar{10, espgui::tft.height()/2, espgui::tft.width()-20, 30, 0, 100};
};
