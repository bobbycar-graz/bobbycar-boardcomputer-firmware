#pragma once

// system includes
#include <optional>

// 3rdparty lib includes
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbydisplaywithtitle.h"

namespace bobby {

class SetupBasicButtonsDisplay : public virtual BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;

    enum CurrentButton : int8_t
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        FINISHED
    }; // button calibration
public:
    explicit SetupBasicButtonsDisplay(bool early_return = false) :
        m_early_return{early_return}
    {}

    void initScreen(espgui::TftInterface &tft) override;
    void start() override;
    void update() override;
    void redraw(espgui::TftInterface &tft) override;

    void rawButtonPressed(uint8_t button) override;
    void rawButtonReleased(uint8_t button) override;
    void buttonPressed(espgui::Button button) override;
    void buttonReleased(espgui::Button button) override;

    [[nodiscard]] std::string text() const override;
private:
    const bool m_early_return;

    std::optional<uint8_t> m_lastButton;

    CurrentButton m_button_cal_status;

    uint8_t m_leftButton, m_rightButton, m_upButton, m_downButton;

    bool m_button_cal_finished;

    bool m_button_rerender_needed;

    void saveButtons() const;

    void drawButtons(espgui::TftInterface &tft, CurrentButton button);
};
} // namespace bobby
