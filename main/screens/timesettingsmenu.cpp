#include "timesettingsmenu.h"

#include "sdkconfig.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <changevaluedisplay_daylightsavingmode.h>
#include <changevaluedisplay_sntp_sync_mode_t.h>
#include <changevaluedisplay_string.h>
#include <espchrono.h>
#include <espstrutils.h>
#include <fmt/core.h>
#include <sunset.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "screens/settingsmenu.h"
#include "utils.h"

namespace bobby {

namespace {
constexpr char TEXT_TIME[] = "Time";
constexpr char TEXT_OFFSET[] = "Offset";
constexpr char TEXT_DAYLIGHTSAVINGMODE[] = "Daylight Saving";
constexpr char TEXT_NTPENABLED[] = "NTP Enabled";
constexpr char TEXT_NTPSERVER[] = "NTP Server";
constexpr char TEXT_NTPMODE[] = "NTP Mode";
constexpr char TEXT_NTPINTERVAL[] = "NTP Interval";
constexpr char TEXT_BACK[] = "Back";

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
#else // Mir egal ob die lokalzeit richtig is
        return fmt::format("local: {}", espchrono::toString(espchrono::toDateTime(espchrono::utc_clock::now() + configs.timezoneOffset.value())));
#endif
    }
};

class SuntimeText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        if (!sunrise && !sunset)
        {
            return "no sunrise/sunset";
        }
        return fmt::format("sunrise: {:02d}:{:02d} sunset: {:02d}:{:02d}", *sunrise / 60, *sunrise % 60, *sunset / 60, *sunset % 60);
    }
};

using TimezoneOffsetChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int32_t>,
    espgui::StaticText<TEXT_OFFSET>,
    TimezoneOffsetAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using DaylightSavingModeChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<espchrono::DayLightSavingMode>,
    espgui::StaticText<TEXT_DAYLIGHTSAVINGMODE>,
    DaylightSavingModeAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using TimeServerChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticText<TEXT_NTPSERVER>,
    TimeServerAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using TimeSyncModeChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<sntp_sync_mode_t>,
    espgui::StaticText<TEXT_NTPMODE>,
    TimeSyncModeAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using TimeSyncIntervalChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int32_t>,
    espgui::StaticText<TEXT_NTPINTERVAL>,
    TimeSyncIntervalAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

class NtpSyncStatusText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("Status: {}", espcpputils::toString(sntp_get_sync_status()));
    }
};
} // namespace

TimeSettingsMenu::TimeSettingsMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, CurrentUtcDateTimeText,              StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CurrentLocalDateTimeText,            StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, SuntimeText,                         StaticFont<2>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_OFFSET>,             PushScreenAction<TimezoneOffsetChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DAYLIGHTSAVINGMODE>, PushScreenAction<DaylightSavingModeChangeDisplay>>>();
    if (configs.feature.ntp.isEnabled.value())
    {
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPENABLED>,         BobbyCheckbox, TimeServerEnabledAccessor>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPSERVER>,          PushScreenAction<TimeServerChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPMODE>,            PushScreenAction<TimeSyncModeChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_NTPINTERVAL>,        PushScreenAction<TimeSyncIntervalChangeDisplay>>>();
        constructMenuItem<makeComponent<MenuItem, NtpSyncStatusText,                   DummyAction>>();
    }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,               PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string TimeSettingsMenu::text() const
{
    return TEXT_TIME;
}

void TimeSettingsMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby
