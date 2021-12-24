#include "timesettingsmenu.h"

#include "sdkconfig.h"

// 3rdparty lib includes
#include <espchrono.h>
#include <fmt/core.h>
#include "changevaluedisplay.h"
#include "changevaluedisplay_daylightsavingmode.h"
#include "changevaluedisplay_sntp_sync_mode_t.h"
#include "actions/toggleboolaction.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "espstrutils.h"

// local includes
#include "utilities/utils.h"
#include "accessors/settingsaccessors.h"
#include "displays/menus/settingsmenu.h"

class CurrentUtcDateTimeText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("utc: {}", espchrono::toString(espchrono::toDateTime(espchrono::utc_clock::now())));
    }
};

class CurrentLocalDateTimeText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
#ifdef CONFIG_ESPCHRONO_SUPPORT_DEFAULT_TIMEZONE
        return fmt::format("local: {}", espchrono::toString(espchrono::toDateTime(espchrono::local_clock::now())));
#else
        // Crude local time implementation
        return fmt::format("local: {}", espchrono::toString(espchrono::toDateTime(espchrono::utc_clock::now() + settings.timeSettings.timezoneOffset)));
#endif
    }
};

using TimezoneOffsetChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int32_t>,
    espgui::StaticText<TEXT_OFFSET>,
    TimezoneOffsetAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<TimeSettingsMenu>>,
    espgui::SwitchScreenAction<TimeSettingsMenu>
>;

using DaylightSavingModeChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<espchrono::DayLightSavingMode>,
    espgui::StaticText<TEXT_DAYLIGHTSAVINGMODE>,
    DaylightSavingModeAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<TimeSettingsMenu>>,
    espgui::SwitchScreenAction<TimeSettingsMenu>
>;

#ifdef FEATURE_NTP
using TimeSyncModeChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<sntp_sync_mode_t>,
    espgui::StaticText<TEXT_NTPMODE>,
    TimeSyncModeAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<TimeSettingsMenu>>,
    espgui::SwitchScreenAction<TimeSettingsMenu>
>;

using TimeSyncIntervalChangeDisplay = espgui::makeComponent<
    espgui::ChangeValueDisplay<int32_t>,
    espgui::StaticText<TEXT_NTPINTERVAL>,
    TimeSyncIntervalAccessor,
    espgui::BackActionInterface<espgui::SwitchScreenAction<TimeSettingsMenu>>,
    espgui::SwitchScreenAction<TimeSettingsMenu>
>;

class NtpSyncStatusText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("Status: {}", espcpputils::toString(sntp_get_sync_status()));
    }
};
#endif

using namespace espgui;

TimeSettingsMenu::TimeSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, CurrentUtcDateTimeText,              StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CurrentLocalDateTimeText,            StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_OFFSET>,             SwitchScreenAction<TimezoneOffsetChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DAYLIGHTSAVINGMODE>, SwitchScreenAction<DaylightSavingModeChangeDisplay>>>();
#ifdef FEATURE_NTP
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPENABLED>,         ToggleBoolAction, CheckboxIcon, TimeServerEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPSERVER>,          DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPMODE>,            SwitchScreenAction<TimeSyncModeChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPINTERVAL>,        SwitchScreenAction<TimeSyncIntervalChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, NtpSyncStatusText,                   DummyAction>>();
#endif
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void TimeSettingsMenu::back()
{
    switchScreen<SettingsMenu>();
}
