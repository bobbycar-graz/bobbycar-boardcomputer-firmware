#include "information.h"

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftinterface.h>

// local includes
#include "screens/setup/basic_buttons.h"
#include "setup.h"
#include "utils.h"

namespace bobby {

using namespace std::chrono_literals;

namespace {
constexpr char const informationText[] = "Congratulations on your new\nbobbycar! This guide will help\nyou through initial setup,\ncalibrate everything and\nget you ready!";
} // namespace

void SetupInformationDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    m_init_text_progressbar.construct(10, tft.height()/2, tft.width()-20, 30, 0, 100);

    m_init_text_progressbar->start(tft);

    drawLargeText(tft, informationText);
}

void SetupInformationDisplay::start()
{
    Base::start();

    setup::lock();

    m_menu_opened_timestamp = espchrono::millis_clock::now();
}

void SetupInformationDisplay::update()
{
    Base::update();

    if (espchrono::ago(m_menu_opened_timestamp) > 5s)
    {
        espgui::switchScreen<SetupBasicButtonsDisplay>();
    }
}

void SetupInformationDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    m_init_text_progressbar->redraw(tft, espchrono::ago(m_menu_opened_timestamp) / 50ms);
}

void SetupInformationDisplay::buttonPressed(espgui::Button button)
{
    if (espchrono::ago(m_menu_opened_timestamp) > 500ms)
    {
        espgui::switchScreen<SetupBasicButtonsDisplay>();
    }
}

[[nodiscard]] std::string SetupInformationDisplay::text() const
{
    return "First Steps";
}
} // namespace bobby
