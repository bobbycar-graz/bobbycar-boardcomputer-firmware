#include "aboutmenu.h"

using namespace espgui;

AboutMenu::AboutMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_VERSION>,                                   DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, HeapTotal8Text,               StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, HeapFree8Text,                StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, HeapMinFree8Text,             StaticFont<2>, DisabledColor, DummyAction>>();
#ifndef HEAP_LRGST_CRASH_TEXT_FIX
    constructMenuItem<makeComponent<MenuItem, HeapLargest8Text,             StaticFont<2>, DisabledColor, DummyAction>>();
#endif
    constructMenuItem<makeComponent<MenuItem, HeapTotal32Text,              StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, HeapFree32Text,               StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, HeapMinFree32Text,            StaticFont<2>, DisabledColor, DummyAction>>();
#ifndef HEAP_LRGST_CRASH_TEXT_FIX
    constructMenuItem<makeComponent<MenuItem, HeapLargest32Text,            StaticFont<2>, DisabledColor, DummyAction>>();
#endif
    constructMenuItem<makeComponent<MenuItem, EspChipRevisionText,          StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspCpuFreqMHzText,            StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspCycleCountText,            StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspSdkVersionText,            StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspFlashChipSizeText,         StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspFlashChipSpeedText,        StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspFlashChipModeText,         StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspSketchSizeText,            StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspSketchMd5Text,             StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EspFreeSketchSpaceText,       StaticFont<2>, DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,        SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
