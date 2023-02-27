#pragma once

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"

namespace bobby {

class SetupAskCalibrateOtherButtonsDisplay : public virtual BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;
public:
    void initScreen(espgui::TftInterface &tft) override;
    void start() override;

    void buttonPressed(espgui::Button button) override;

    [[nodiscard]] std::string text() const override;
private:
    bool m_next_screen{false};
};
} // namespace bobby
