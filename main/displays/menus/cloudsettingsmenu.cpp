#include "cloudsettingsmenu.h"

using namespace espgui;

#ifdef FEATURE_CLOUD
using CloudTransmitTimeoutChangeScreen = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_CLOUDTRANSMITTIMEOUT>,
    CloudTransmitTimeoutAccessor,
    BackActionInterface<SwitchScreenAction<CloudSettingsMenu>>,
    SwitchScreenAction<CloudSettingsMenu>
>;

struct CloudBufferLengthText : public virtual TextInterface {
public:
    std::string text() const override
    {
        return fmt::format("buffer: {}", cloudBuffer.size());
    }
};

using CloudCollectRateChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_CLOUDCOLLECTRATE>,
    CloudCollectRateAccessor,
    BackActionInterface<SwitchScreenAction<CloudSettingsMenu>>,
    SwitchScreenAction<CloudSettingsMenu>
>;

using CloudSendRateChangeDisplay = makeComponent<
    ChangeValueDisplay<int16_t>,
    StaticText<TEXT_CLOUDSENDRATE>,
    CloudSendRateAccessor,
    BackActionInterface<SwitchScreenAction<CloudSettingsMenu>>,
    SwitchScreenAction<CloudSettingsMenu>
>;

CloudSettingsMenu::CloudSettingsMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDENABLED>,         ToggleBoolAction, CheckboxIcon, CloudEnabledAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDTRANSMITTIMEOUT>, SwitchScreenAction<CloudTransmitTimeoutChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, CloudCreatedText,                      DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CloudStartedText,                      DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CloudConnectedText,                    DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, CloudBufferLengthText,                 DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDCOLLECTRATE>,     SwitchScreenAction<CloudCollectRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLOUDSENDRATE>,        SwitchScreenAction<CloudSendRateChangeDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                 SwitchScreenAction<SettingsMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}
#endif
