#include "commanddebugmenu.h"

using namespace espgui;

template<const char *Ttext, typename Ttexts>
CommandDebugMenu<Ttext, Ttexts>::CommandDebugMenu()
{
    constructMenuItem<makeComponent<MenuItem, typename Ttexts::BuzzerFreqText,    DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, typename Ttexts::BuzzerPatternText, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, typename Ttexts::PoweroffText,      DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, typename Ttexts::LedText,           DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,              SwitchScreenAction<DebugMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
