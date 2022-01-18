#include "aboutmenu.h"

// local includes
#include "utils.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "esptexthelpers.h"
#include "displays/menus/settingsmenu.h"

#include <espasyncota.h>
#include <esp_ota_ops.h>
#include "fmt/core.h"

namespace {
constexpr char TEXT_ABOUT[] = "About";
constexpr char TEXT_BACK[] = "Back";

class CurrentVersionText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        if (const esp_app_desc_t *app_desc = esp_ota_get_app_description())
        {
            return fmt::format("Version: {}", app_desc->version);
        }
        return "Version: Unkown";
    };
};
} // namespace

AboutMenu::AboutMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, CurrentVersionText,                                         DummyAction>>();
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

std::string AboutMenu::text() const
{
    return TEXT_ABOUT;
}

void AboutMenu::back()
{
    espgui::switchScreen<SettingsMenu>();
}
