#include "setup_cloud.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay_string.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "guihelpers/bobbycheckbox.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "screens/setup/ask_calibrate_other_buttons.h"
#include "setup.h"

namespace bobby {

using namespace espgui;

namespace {
constexpr const char TEXT_ENABLE_CLOUD[] = "Enable Cloud";
constexpr const char TEXT_CLOUD_USERNAME[] = "Cloud Username";
constexpr const char TEXT_CLOUD_URL[] = "Cloud URL";
constexpr const char TEXT_CLOUD_KEY[] = "Cloud Key";
constexpr const char TEXT_DONE[] = "Done";

using CloudUsernameChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticText<TEXT_CLOUD_USERNAME>,
    UsernameAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using CloudURLChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticText<TEXT_CLOUD_URL>,
    CloudURLAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using CloudKeyChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<std::string>,
    espgui::StaticText<TEXT_CLOUD_KEY>,
    CloudKeyAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>; // cloud setup

template<bool early_return>
class CloudSetupFinishedAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        if (early_return)
        {
            espgui::popScreen();
        }
        else
        {
            espgui::switchScreen<SetupAskCalibrateOtherButtonsDisplay>();
        }
    }
};
} // namespace

SetupCloudDisplay::SetupCloudDisplay(const bool early_return) : m_early_return{early_return}
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ENABLE_CLOUD>, BobbyCheckbox, CloudEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUD_USERNAME>, PushScreenAction<CloudUsernameChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUD_URL>, PushScreenAction<CloudURLChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUD_KEY>, PushScreenAction<CloudKeyChangeScreen>>>();

    if (early_return)
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DONE>, CloudSetupFinishedAction<true>>>();
    else
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DONE>, CloudSetupFinishedAction<false>>>();
}

void SetupCloudDisplay::start()
{
    Base::start();

    setup::lock();
}

void SetupCloudDisplay::stop()
{
    if (m_early_return)
    {
        setup::unlock();
    }

    Base::stop();
}

void SetupCloudDisplay::back()
{}

std::string SetupCloudDisplay::text() const
{
    return "WebSocket Cloud";
}
} // namespace bobby
