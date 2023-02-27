#include "cloudsettingsmenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <changevaluedisplay.h>
#include <changevaluedisplay_string.h>
#include <fmt/core.h>
#include <menuitem.h>


// local includes
#include "accessors/settingsaccessors.h"
#include "cloud.h"
#include "guihelpers/bobbychangevaluedisplay.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "screens/settingsmenu.h"
#include "texthelpers/cloudtexthelpers.h"

namespace bobby {

namespace {
constexpr char TEXT_CLOUDSETTINGS[] = "Cloud settings";
constexpr char TEXT_CLOUDURL[] = "Cloud URL";
constexpr char TEXT_CLOUDKEY[] = "Cloud Key";
constexpr char TEXT_CLOUDENABLED[] = "Cloud enabled";
constexpr char TEXT_CLOUDTRANSMITTIMEOUT[] = "Transmit timeout";
constexpr char TEXT_SENDSTATISTICS[] = "Send Statistics";
constexpr char TEXT_CLOUDCOLLECTRATE[] = "Cloud collect rate";
constexpr char TEXT_CLOUDSENDRATE[] = "Cloud send rate";
constexpr char TEXT_BACK[] = "Back";

using CloudURLChangeScreen = espgui::makeComponent<
        BobbyChangeValueDisplay<std::string>,
        espgui::StaticText<TEXT_CLOUDURL>,
        CloudURLAccessor,
        espgui::ConfirmActionInterface<espgui::PopScreenAction>,
        espgui::BackActionInterface<espgui::PopScreenAction>
>;

using CloudKeyChangeScreen = espgui::makeComponent<
        BobbyChangeValueDisplay<std::string>,
        espgui::StaticText<TEXT_CLOUDKEY>,
        CloudKeyAccessor,
        espgui::ConfirmActionInterface<espgui::PopScreenAction>,
        espgui::BackActionInterface<espgui::PopScreenAction>
>;

using CloudTransmitTimeoutChangeScreen = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_CLOUDTRANSMITTIMEOUT>,
    CloudTransmitTimeoutAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

struct CloudBufferLengthText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("buffer: {}", cloudBuffer.size());
    }
};

using CloudCollectRateChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_CLOUDCOLLECTRATE>,
    CloudCollectRateAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

using CloudSendRateChangeDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<int16_t>,
    espgui::StaticText<TEXT_CLOUDSENDRATE>,
    CloudSendRateAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;
} // namespace

using namespace espgui;

CloudSettingsMenu::CloudSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDENABLED>,         BobbyCheckbox, CloudEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDURL>,             PushScreenAction<CloudURLChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDKEY>,             PushScreenAction<CloudKeyChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDTRANSMITTIMEOUT>, PushScreenAction<CloudTransmitTimeoutChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_SENDSTATISTICS>,       BobbyCheckbox, CloudSendStatisticsAccessor>>();
    constructMenuItem<makeComponent<MenuItem, CloudCreatedText,                      DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CloudStartedText,                      DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CloudConnectedText,                    DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CloudBufferLengthText,                 DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDCOLLECTRATE>,     PushScreenAction<CloudCollectRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDSENDRATE>,        PushScreenAction<CloudSendRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 PushScreenAction<SettingsMenu>, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string CloudSettingsMenu::text() const
{
    return TEXT_CLOUDSETTINGS;
}

void CloudSettingsMenu::back()
{
    popScreen();
}

} // namespace bobby
