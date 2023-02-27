#pragma once

// system libs
#include <optional>

// 3rdparty lib includes
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"
#include "modeinterface.h"
#include "modes/ignoreinputmode.h"

#ifdef FEATURE_JOYSTICK

namespace bobby {

class JoystickDebugDisplay : public BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

public:
    JoystickDebugDisplay();

    std::string text() const override;
    void start() override;
    void update() override;
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

    void copyFromSettings();
    void copyToSettings();
private:
    std::optional<int16_t> m_x;
    std::optional<int16_t> m_y;

    int16_t m_gasMin, m_gasMax, m_bremsMin, m_bremsMax, m_gasMitte, m_bremsMitte;

    bool m_clear{false};
};
} // namespace bobby
#endif
