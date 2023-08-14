#pragma once

// local includes
#include "displays/bobbydisplaywithtitle.h"

class SetupAskCalibrateOtherButtonsDisplay : public virtual BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;
public:
    void initScreen(espgui::TftInterface &tft) override;
    void start() override;

    void buttonPressed(espgui::Button button) override;

    [[nodiscard]] std::string title() const override;
private:
    bool m_next_screen{false};
};
