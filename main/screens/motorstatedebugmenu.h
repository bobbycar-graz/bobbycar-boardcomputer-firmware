#pragma once

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <menuitem.h>

// local includes
#include "guihelpers/bobbymenudisplay.h"
#include "icons/back.h"
#include "texthelpers/debugtexthelpers.h"
#include "utils.h"

namespace bobby {

template<const char *Ttext, typename Ttexts>
class MotorStateDebugMenu :
    public BobbyMenuDisplay,
    public espgui::StaticText<Ttext>,
    public espgui::BackActionInterface<espgui::PopScreenAction>
{
public:
    MotorStateDebugMenu()
    {
        using namespace espgui;
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::EnableText,        DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::PwmText,           DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::CtrlTypText,       DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::CtrlModText,       DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::IMotMaxText,       DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::IDcMaxText,        DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::NMotMaxText,       DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::FieldWeakMaxText,  DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::PhaseAdvMaxText,   DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::CruiseCtrlEnaText, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, typename Ttexts::NCruiseMotTgtText, DisabledColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,    PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
    }

private:
    static constexpr char TEXT_BACK[] = "Back";
};

constexpr char TEXT_FRONTLEFTCOMMAND[] = "Front left command";
using FrontLeftMotorStateDebugMenu = MotorStateDebugMenu<TEXT_FRONTLEFTCOMMAND, FrontTexts::LeftCommand>;

constexpr char TEXT_FRONTRIGHTCOMMAND[] = "Front right command";
using FrontRightMotorStateDebugMenu = MotorStateDebugMenu<TEXT_FRONTRIGHTCOMMAND, FrontTexts::RightCommand>;

constexpr char TEXT_BACKLEFTCOMMAND[] = "Back left command";
using BackLeftMotorStateDebugMenu = MotorStateDebugMenu<TEXT_BACKLEFTCOMMAND, BackTexts::LeftCommand>;

constexpr char TEXT_BACKRIGHTCOMMAND[] = "Back right command";
using BackRightMotorStateDebugMenu = MotorStateDebugMenu<TEXT_BACKRIGHTCOMMAND, BackTexts::RightCommand>;

} // namespace bobby
