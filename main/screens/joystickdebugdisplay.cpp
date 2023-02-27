#ifdef FEATURE_JOYSTICK
#include "joystickdebugdisplay.h"

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "screens/statusdisplay.h"
#include "screens/boardcomputerhardwaresettingsmenu.h"
#include "newsettings.h"
#include "utils.h"
#include "globals.h"

namespace bobby {

using namespace espgui;

namespace {
constexpr char TEXT_JOYSTICK[] = "Joystick Debug";
} // namespace

JoystickDebugDisplay::JoystickDebugDisplay()
{
}

void JoystickDebugDisplay::start()
{
    copyFromSettings();
}

std::string JoystickDebugDisplay::text() const
{
    return TEXT_JOYSTICK;
}

void JoystickDebugDisplay::update()
{
    Base::update();

    if (!raw_gas)
        m_x = std::nullopt;
    else
    {
        m_x = map_analog_stick(m_gasMitte, m_gasMin, m_gasMax, configs.deadband.value(), *raw_gas);
    }

    if (!raw_brems)
        m_y = std::nullopt;
    else
        m_y = map_analog_stick(m_bremsMitte, m_bremsMin, m_bremsMax, configs.deadband.value(), *raw_brems);
}

void JoystickDebugDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    if (m_x && m_y)
    {
        tft.drawPixel(
                    (tft.width() / 2) + *m_x / 10,
                    (tft.height() / 2) + *m_y / 10,
                    espgui::TFT_WHITE);
    }

    if (m_clear)
    {
        tft.fillScreen(espgui::TFT_BLACK);
        m_clear = false;
    }
}

void JoystickDebugDisplay::copyFromSettings()
{
    m_gasMitte = configs.gasMitte.value();
    m_gasMin = configs.gasMin.value();
    m_gasMax = configs.gasMax.value();
    m_bremsMitte = configs.bremsMitte.value();
    m_bremsMin = configs.bremsMin.value();
    m_bremsMax = configs.bremsMax.value();
}

void JoystickDebugDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
        using espgui::Button;
    case Button::Right:
        m_clear = true;
        break;
    case Button::Left:
        espgui::popScreen();
        break;
    default:;
    }
}
} // namespace bobby
#endif
