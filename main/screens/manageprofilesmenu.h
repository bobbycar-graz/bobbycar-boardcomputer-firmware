#pragma once

// 3rdparty lib includes
#include "cpptypesafeenum.h"

// local includes
#include "guihelpers/bobbymenudisplay.h"
#include "modes/ignoreinputmode.h"

namespace bobby {

#define ActionValues(x) \
    x(Clear) \
    x(Copy) \
    x(Swap) \
    x(__END__)
DECLARE_TYPESAFE_ENUM(Actions, : uint8_t, ActionValues)

class ManageProfilesMenu : public BobbyMenuDisplay
{
    friend class ManageProfileMenuItem;
    friend class ManageProfileModeMenuItem;
    using Base = BobbyMenuDisplay;
public:
    ManageProfilesMenu();
    void start() override;
    void stop() override;

    std::string text() const override;
    std::string action_text() const;
    void lock();
    void unlock();

    void back() override {};

    void buttonPressed(espgui::Button button) override;
private:
    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Torque};
protected:
    bool m_locked{false};
    Actions m_action{Actions::Clear};
    int8_t m_firstIndex{-1};
};
} // namespace bobby
