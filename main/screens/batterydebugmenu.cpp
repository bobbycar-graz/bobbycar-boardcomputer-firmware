#include "batterydebugmenu.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <fmt/core.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "battery.h"
#include "icons/back.h"
#include "icons/settings.h"
#include "utils.h"

namespace bobby {

namespace {
constexpr char TEXT_BATTERYDEBUG[] = "Bat Debug Menu";
constexpr char TEXT_BACK[] = "Back";

class CurrentBatteryStatusText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return getBatteryPercentageString();
    }
};

class CurrentAdvancedBatteryPercentageText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return getBatteryAdvancedPercentageString();
    }
};

class BatteryDebugText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return getBatteryDebugString();
    }
};

class BatteryDebug2Text : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        if (const auto avgVoltage = controllers.getAvgVoltage(); avgVoltage)
        {
            auto watt = sumCurrent * *avgVoltage;
            auto w_per_kmh = watt / avgSpeedKmh;
            return fmt::format("{:.0f} {:.0f}W/kmh", avgSpeedKmh, w_per_kmh);
        }
        else
            return "No battery";
    }
};

class BatteryDebug3Text : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("{}fA {}bA",
                           fixCurrent(controllers.front.feedback.left.dcLink + controllers.front.feedback.right.dcLink),
                           fixCurrent(controllers.back.feedback.left.dcLink + controllers.back.feedback.right.dcLink));
    }
};
} // namespace

BatteryDebugMenu::BatteryDebugMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, CurrentBatteryStatusText,                                                 DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, EmptyText,                                                                DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BatteryDebugText,                                                         DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BatteryDebug2Text,                                                        DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BatteryDebug3Text,                                                        DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CurrentAdvancedBatteryPercentageText,                                     DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                                                    PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string BatteryDebugMenu::text() const
{
    return TEXT_BATTERYDEBUG;
}

void BatteryDebugMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby
