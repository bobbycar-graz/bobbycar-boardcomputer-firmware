#include "confiscationdisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>
#include <actions/switchscreenaction.h>
#include <esprandom.h>
#include <randomutils.h>

// local includes
#include "globals.h"
#include "displays/menus/mainmenu.h"
#include "icons/shortcircuit.h"

using namespace std::chrono_literals;

#if defined(FEATURE_CAN) && defined(FEATURE_POWERSUPPLY)
void ConfiscationDisplay::start()
{
    Base::start();

    m_oldMode = currentMode;
    currentMode = &m_mode;

    m_progress = 500;

    m_nextRestart = espchrono::millis_clock::now() + std::chrono::seconds{cpputils::randomNumber(3, 7, espcpputils::esp_random_device{})};
}

void ConfiscationDisplay::initScreen()
{
    Base::initScreen();

    espgui::tft.setSwapBytes(true);
    espgui::tft.pushImage(10, 70, bobbyicons::shortcircuit.WIDTH, bobbyicons::shortcircuit.HEIGHT, bobbyicons::shortcircuit.buffer);
    espgui::tft.setSwapBytes(false);

    m_progressBar.start();

    m_label.start();

    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);
    espgui::tft.setTextFont(2);

    auto y = 235;
    constexpr auto lineheight = 15;
    espgui::tft.drawString("Bei erneuter, widerrechtlicher", 10, y+=lineheight);
    espgui::tft.drawString("Beschlagnahmung wird die Selbst-", 10, y+=lineheight);
    espgui::tft.drawString("Vernichtung durch Kurzschluss", 10, y+=lineheight);
    espgui::tft.drawString("der Batterie eingeleitet (ca 4.31MJ)", 10, y+=lineheight);
}

void ConfiscationDisplay::redraw()
{
    Base::redraw();

    m_progressBar.redraw(m_progress);

    espgui::tft.setTextColor(TFT_YELLOW, TFT_BLACK);

    espgui::tft.setTextFont(2);
    m_label.redraw([](){
        if (const auto period = espchrono::millis_clock::now().time_since_epoch() % 6000ms; period < 2000ms)
            return "Halten Sie 10m Abstand.";
        else if (period < 4000ms)
            return "Bewegen Sie kein Rad.";
        else
            return "Die Lenkung nicht drehen.";
    }());
}

void ConfiscationDisplay::update()
{
    if (espchrono::millis_clock::now() > m_nextRestart)
    {
        m_progress = 500;
        const auto timeout = cpputils::randomNumber(3, 7, espcpputils::esp_random_device{});
        ESP_LOGI("BOBBY", "agian in %i", timeout);
        m_nextRestart = espchrono::millis_clock::now() + std::chrono::seconds{timeout};
    }
    else
        m_progress--;
}

void ConfiscationDisplay::stop()
{
    Base::stop();

    if (currentMode == &m_mode)
    {
        // to avoid crash after deconstruction
        m_mode.stop();
        lastMode = nullptr;

        currentMode = m_oldMode;
    }
}

void ConfiscationDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
        using espgui::Button;
    case Button::Left:
        espgui::switchScreen<MainMenu>();
        break;
    default:;
    }
}

std::string ConfiscationDisplay::text() const
{
    return "Explosions-Modus";
}
#endif
