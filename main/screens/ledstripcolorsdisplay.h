#pragma once

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"
#include "ledstrip.h"

namespace bobby {

class LedstripColorsDisplay : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    std::string text() const override;
    void initScreen(espgui::TftInterface &tft) override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

    void drawColors(espgui::TftInterface &tft);
    void drawSide(espgui::TftInterface &tft, Bobbycar_Side side, unsigned int color);
    void clearSides(espgui::TftInterface &tft);

private:
    bool already_drew_circle{false};
};

} // namespace bobby
