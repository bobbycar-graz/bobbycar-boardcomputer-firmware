#pragma once

// system includes
#include <array>

// 3rdparty lib includes
#include <widgets/label.h>

// local includes
#include "guihelpers/bobbydisplay.h"
#include "modes/ignoreinputmode.h"

#ifdef LOCKSCREEN_PLUGIN
#include "ledstrip.h"
#endif

namespace bobby {

class Lockscreen : public BobbyDisplay
{
    using Base = BobbyDisplay;

    static constexpr auto boxWidth = 35;
    static constexpr auto boxHeight = 50;
    static constexpr auto spacing = 20;

public:
    void start() override;
    void initScreen(espgui::TftInterface &tft) override;
    void update() override;
    void redraw(espgui::TftInterface &tft) override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

private:
    void drawRect(espgui::TftInterface &tft, int index, int offset, uint32_t color);

    std::array<espgui::Label, 4> m_labels {{
        espgui::Label{spacing, 100}, // boxWidth, boxHeight
        espgui::Label{spacing*2+boxWidth, 100}, // boxWidth, boxHeight
        espgui::Label{spacing*3+boxWidth*2, 100}, // boxWidth, boxHeight
        espgui::Label{spacing*4+boxWidth*3, 100} // boxWidth, boxHeight
    }};

    std::array<int8_t, 4> m_numbers;

    uint8_t m_currentIndex{};

    bool m_pressed;
    bool m_back_pressed;
    int m_rotated;

    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Speed};
};

} // namespace bobby
