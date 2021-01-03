#pragma once

// local includes
#include "menudisplay.h"
#include "utils.h"
#include "actions/dummyaction.h"
#include "actions/switchscreenaction.h"
#include "icons/back.h"
#include "esptexthelpers.h"
#include "texts.h"

// forward declares
namespace {
class SettingsMenu;
} // namespace

namespace {
constexpr char TEXT_VERSION[] = "Version: 1.0";
constexpr char TEXT_ESPINFO[] = "ESP info:";

class AboutMenu :
    public MenuDisplay,
    public StaticText<TEXT_ABOUT>,
    public BackActionInterface<SwitchScreenAction<SettingsMenu>>
{
public:
    AboutMenu()
    {
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_VERSION>>());
        constructMenuItem<NewMenuItem>();
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_ESPINFO>>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspHeapSizeText>(),        std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspFreeHeapText>(),        std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspMinFreeHeapText>(),     std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspMaxAllocHeapText>(),    std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspPsramSizeText>(),       std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspFreePsramText>(),       std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspMinFreePsramText>(),    std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspMaxAllocPsramText>(),   std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspChipRevisionText>(),    std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspCpuFreqMHzText>(),      std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspCycleCountText>(),      std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspSdkVersionText>(),      std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspFlashChipSizeText>(),   std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspFlashChipSpeedText>(),  std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspFlashChipModeText>(),   std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspSketchSizeText>(),      std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspSketchMd5Text>(),       std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<EspFreeSketchSpaceText>(), std::make_unique<ConstantFont>(2), std::make_unique<DisabledColor>());
        constructMenuItem<NewMenuItem>(std::make_unique<StaticText<TEXT_BACK>>(),  std::make_unique<SwitchScreenAction<SettingsMenu>>(), std::make_unique<StaticMenuItemIcon<&icons::back>>());
    }
};
} // namespace
