#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "esptexthelpers.h"
#include "texts.h"
#ifdef FEATURE_OTA
#include <espasyncota.h>
#include <esp_ota_ops.h>
#include "fmt/core.h"
#endif

// forward declares
namespace {
class SettingsMenu;
} // namespace

using namespace espgui;

namespace {

class currentVersionText : public virtual TextInterface { public: std::string text() const override {
#ifdef FEATURE_OTA
        if (const esp_app_desc_t *app_desc = esp_ota_get_app_description())
        {
            return fmt::format("Version: {}", app_desc->version);
        }
#endif
        return "Version: 1.0";
    };
};

constexpr char TEXT_VERSION[] = "Version: 1.0";

class AboutMenu :
    public MenuDisplay,
    public StaticText<TEXT_ABOUT>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>
{
public:
    AboutMenu()
    {
        constructMenuItem<makeComponent<MenuItem, currentVersionText,                                         DummyAction>>();
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
};
} // namespace
