#include "lockscreen.h"

// system includes
#include <algorithm>

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "bobbybuttons.h"
#include "screens/mainmenu.h"
#include "globals.h"
#include "utils.h"

namespace bobby {

namespace {
bool isValid1stPin(std::array<int8_t, 4> enteredPin)
{
    return std::equal(std::cbegin(enteredPin), std::cend(enteredPin),
                      std::cbegin(configs.lockscreen.pin), std::cend(configs.lockscreen.pin),
                      [](const int8_t digit, const auto &configuredDigit){
                          return digit == configuredDigit.value();
                      });
}

bool isValid2ndPin(std::array<int8_t, 4> enteredPin)
{
    return std::equal(std::cbegin(enteredPin), std::cend(enteredPin),
                      std::cbegin(configs.lockscreen.pin2), std::cend(configs.lockscreen.pin2),
                      [](const int8_t digit, const auto &configuredDigit){
                          return digit == configuredDigit.value();
                      });
}
} // namespace

void Lockscreen::start()
{
    Base::start();

    m_numbers = {0,0,0,0};
    m_currentIndex = 0;
    m_pressed = false;
    m_rotated = 0;

    m_oldMode = currentMode;
    currentMode = &m_mode;

    isLocked = true;
    if (configs.lockscreen.keepLockedAfterReboot.value() && !configs.lockscreen.locked.value())
    {
        configs.write_config(configs.lockscreen.locked, true);
    }
}

void Lockscreen::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.fillScreen(espgui::TFT_BLACK);

    tft.drawString("Lock vehicle", 5, 5, espgui::TFT_YELLOW, espgui::TFT_BLACK, 4);

    tft.fillRect(0, 34, tft.width(), 3, espgui::TFT_WHITE);

    tft.drawString("Enter code to unlock:", 0, 50, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    for(int i = 0; i <= 3; i++)
    {
        drawRect(tft, i, 3, espgui::TFT_WHITE);
        drawRect(tft, i, 4, espgui::TFT_WHITE);
    }

    for (auto &label : m_labels)
        label.start(tft);

    drawRect(tft, m_currentIndex, 1, espgui::TFT_YELLOW);
    drawRect(tft, m_currentIndex, 2, espgui::TFT_YELLOW);

    for(int i = 0; i <= m_currentIndex; i++)
        m_labels[i].redraw(tft, std::to_string(m_numbers[i]), espgui::TFT_WHITE, espgui::TFT_BLACK, 7);
}

void Lockscreen::update()
{
    Base::update();
}

void Lockscreen::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    if (m_pressed || m_back_pressed)
    {
        drawRect(tft, m_currentIndex, 1, espgui::TFT_BLACK);
        drawRect(tft, m_currentIndex, 2, espgui::TFT_BLACK);

        if (!m_back_pressed && ++m_currentIndex>=4)
        {
            if (isValid1stPin(m_numbers))
            {
                espgui::popScreen();
#ifdef LOCKSCREEN_PLUGIN
#include LOCKSCREEN_PLUGIN
LOCKSCREEN_PLUGIN_FIXES_1
#endif
                return;
            }
            else if(isValid2ndPin(m_numbers))
            {
                espgui::popScreen();
#ifdef LOCKSCREEN_PLUGIN_FIXES_2
LOCKSCREEN_PLUGIN_FIXES_2
#endif
                return;
            }

            m_numbers = {0,0,0,0};
            m_currentIndex = 0;
            std::for_each(std::begin(m_labels) + 1, std::end(m_labels), [&](auto &label){ label.redraw(tft, "0", espgui::TFT_WHITE, espgui::TFT_BLACK, 7); });
        }
        else if (m_back_pressed && m_currentIndex < 3)
        {
            drawRect(tft, m_currentIndex+1, 1, espgui::TFT_BLACK);
            drawRect(tft, m_currentIndex+1, 2, TFT_BLACK);
        }

        m_labels[m_currentIndex].redraw(tft, std::to_string(m_numbers[m_currentIndex]), espgui::TFT_WHITE, espgui::TFT_BLACK, 7);

        drawRect(tft, m_currentIndex, 1, espgui::TFT_YELLOW);
        drawRect(tft, m_currentIndex, 2, espgui::TFT_YELLOW);

        m_pressed = false;
        m_back_pressed = false;
    }

    if (m_rotated)
    {
        m_numbers[m_currentIndex] -= m_rotated;

        if (m_numbers[m_currentIndex] < 0)
            m_numbers[m_currentIndex]+=10;
        else if (m_numbers[m_currentIndex] > 9)
            m_numbers[m_currentIndex]-=10;

        m_labels[m_currentIndex].redraw(tft, std::to_string(m_numbers[m_currentIndex]), espgui::TFT_WHITE, espgui::TFT_BLACK, 7);

        m_rotated = 0;
    }
}

void Lockscreen::stop()
{
    Base::stop();

    if (currentMode == &m_mode)
    {
        // to avoid crash after deconstruction
        m_mode.stop();
        lastMode = nullptr;

        currentMode = m_oldMode;
    }

    isLocked = false;
    if (!(!gas || !brems || *gas > 200.f || *brems > 200.f))
    {
        if (configs.lockscreen.keepLockedAfterReboot.value() && configs.lockscreen.locked.value())
        {
            configs.write_config(configs.lockscreen.locked, false);
        }
    }
}

void Lockscreen::buttonPressed(espgui::Button button)
{
    if (configs.lockscreen.allowPresetSwitch.value() ||
        !cpputils::is_in(button, BobbyButton::Profile0, BobbyButton::Profile1, BobbyButton::Profile2, BobbyButton::Profile3))
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Left:
        if (m_currentIndex > 0)
            m_currentIndex--;
        m_back_pressed = true;
        break;
    case Button::Right:
        m_pressed = true;
        break;
    case Button::Up:
        m_rotated--;
        break;
    case Button::Down:
        m_rotated++;
        break;
    }
}

void Lockscreen::drawRect(espgui::TftInterface &tft, int index, int offset, uint32_t color)
{
    tft.drawRect(m_labels[index].x()-offset, m_labels[index].y()-offset, boxWidth+(offset*2), boxHeight+(offset*2), color);
}

} // namespace bobby
