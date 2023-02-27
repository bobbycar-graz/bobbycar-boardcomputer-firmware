#include "final_information.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "screens/extrabuttoncalibratemenu.h"
#include "screens/statusdisplay.h"
#include "newsettings.h"
#include "setup.h"
#include "utils.h"

namespace bobby {

using namespace std::chrono_literals;

namespace {
constexpr char const finalInformationText[] = "Setup is done!\nIf cloud is setup, go to\nhttps://service.bobbycar.cloud/\nand register this bobbycar!\nThis is also used\nto setup udp cloud.\nPress any button to exit.";
} // namespace

void SetupFinalInformationDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    drawLargeText(tft, finalInformationText);
}

void SetupFinalInformationDisplay::start()
{
    Base::start();

    setup::lock();
}

void SetupFinalInformationDisplay::stop()
{
    Base::stop();

    setup::unlock();
}

void SetupFinalInformationDisplay::buttonPressed(espgui::Button button)
{
    configs.write_config(configs.boardcomputerHardware.setupFinished, true);
    if (espgui::displayStack.empty())
    {
        espgui::switchScreen<StatusDisplay>();
    }
    else
    {
        espgui::popScreen();
    }

    Base::buttonPressed(button);
}

std::string SetupFinalInformationDisplay::text() const
{
    return "All done!";
}
} // namespace bobby
