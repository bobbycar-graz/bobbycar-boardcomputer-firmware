#include "information.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "displays/setup/basic_buttons.h"
#include "setup.h"
#include "utils.h"

using namespace std::chrono_literals;

namespace {
constexpr char const informationText[] = "Congratulations on your new\nbobbycar! This guide will help\nyou through initial setup,\ncalibrate everything and\nget you ready!";
} // namespace

void SetupInformationDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    m_init_text_progressbar.start();

    drawLargeText(informationText);
}

void SetupInformationDisplay::start()
{
    Base::start();

    setup::lock();

    m_menu_opened_timestamp = espchrono::millis_clock::now();
}

void SetupInformationDisplay::update()
{
    if (espchrono::ago(m_menu_opened_timestamp) > 5s)
    {
        espgui::switchScreen<SetupBasicButtonsDisplay>();
    }

    Base::update();
}

void SetupInformationDisplay::redraw(espgui::TftInterface &tft)
{
    m_init_text_progressbar.redraw(espchrono::ago(m_menu_opened_timestamp) / 50ms);

    Base::redraw(tft);
}

void SetupInformationDisplay::buttonPressed(espgui::Button button)
{
    if (espchrono::ago(m_menu_opened_timestamp) > 500ms)
    {
        espgui::switchScreen<SetupBasicButtonsDisplay>();
    }
}

[[nodiscard]] std::string SetupInformationDisplay::title() const
{
    return "First Steps";
}

