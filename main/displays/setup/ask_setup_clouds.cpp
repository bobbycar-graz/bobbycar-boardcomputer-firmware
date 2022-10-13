#include "ask_setup_clouds.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "displays/setup/ask_calibrate_other_buttons.h"
#include "displays/setup/setup_cloud.h"
#include "setup.h"
#include "taskmanager.h"
#include "utils.h"

namespace {
constexpr char const askCloudText[] = "Do you want to setup cloud?\nWith this, you will be able\nto send data to graphana,\nremote control things like Buttons\nand NVS and more!\n\nPress LEFT to skip cloud.\nPress RIGHT to setup cloud.";
} // namespace

void SetupAskSetupCloudsDisplay::initScreen()
{
    Base::initScreen();

    drawLargeText(askCloudText);
}

void SetupAskSetupCloudsDisplay::start()
{
    Base::start();

    setup::lock();
}

void SetupAskSetupCloudsDisplay::buttonPressed(espgui::Button button)
{
    switch (button)
    {
    case espgui::Left: // skip cloud setup
        espgui::switchScreen<SetupAskCalibrateOtherButtonsDisplay>();
        return;
    case espgui::Right: // enter cloud setup
        configs.write_config(configs.feature.cloud.isEnabled, true);
        configs.write_config(configs.feature.udpcloud.isEnabled, true);

        reload_tasks();
        espgui::switchScreen<SetupCloudDisplay>();
    default:;
    }

    Base::buttonPressed(button);
}

std::string SetupAskSetupCloudsDisplay::text() const
{
    return "Cloud Setup";
}
