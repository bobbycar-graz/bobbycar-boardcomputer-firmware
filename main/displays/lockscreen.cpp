#include "lockscreen.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>

// local includes
#include "globals.h"
#include "utils.h"
#include "displays/menus/mainmenu.h"
#include "displays/potiscalibratedisplay.h"
#include "bobbybuttons.h"

bool isValidPin(std::array<int8_t,4> enteredPin)
{
    for (int i = 0; i < 4; i++)
    {
        if (enteredPin[i] != configs.lockscreen.pin[i].digit.value)
        {
            return false;
        }
    }
    return true;
}

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
    if (configs.lockscreen.keepLockedAfterReboot.value && !configs.lockscreen.locked.value)
    {
        configs.write_config(configs.lockscreen.locked, true);
    }
}

void Lockscreen::initScreen()
{
    Base::initScreen();

    espgui::tft.fillScreen(TFT_BLACK);
    espgui::tft.setTextFont(4);
    espgui::tft.setTextColor(TFT_YELLOW);

    espgui::tft.drawString("Lock vehicle", 5, 5);

    espgui::tft.fillRect(0, 34, espgui::tft.width(), 3, TFT_WHITE);

    espgui::tft.setTextColor(TFT_WHITE);
    espgui::tft.drawString("Enter code to unlock:", 0, 50);

    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    for(int i = 0; i <= 3; i++)
    {
        drawRect(i, 3, TFT_WHITE);
        drawRect(i, 4, TFT_WHITE);
    }

    for (auto &label : m_labels)
        label.start();

    espgui::tft.setTextFont(7);

    drawRect(m_currentIndex, 1, TFT_YELLOW);
    drawRect(m_currentIndex, 2, TFT_YELLOW);

    for(int i = 0; i <= m_currentIndex; i++)
        m_labels[i].redraw(std::to_string(m_numbers[i]));
}

void Lockscreen::update()
{
    Base::update();
}

void Lockscreen::redraw()
{
    Base::redraw();

    if (m_pressed || m_back_pressed)
    {
        drawRect(m_currentIndex, 1, TFT_BLACK);
        drawRect(m_currentIndex, 2, TFT_BLACK);

        if (!m_back_pressed && ++m_currentIndex>=4)
        {
            if (isValidPin(m_numbers))
            {
                if (!gas || !brems || *gas > 200.f || *brems > 200.f)
                    espgui::switchScreen<PotisCalibrateDisplay>(true);
                else
                    espgui::switchScreen<MainMenu>();
#ifdef LOCKSCREEN_PLUGIN
#pragma message "Activating Lockscreen Plugin"
#include LOCKSCREEN_PLUGIN
#endif
                return;
            }

            m_numbers = {0,0,0,0};
            m_currentIndex = 0;
            std::for_each(std::begin(m_labels) + 1, std::end(m_labels), [](auto &label){ label.redraw("0"); });
        }
        else if (m_back_pressed && m_currentIndex < 3)
        {
            drawRect(m_currentIndex+1, 1, TFT_BLACK);
            drawRect(m_currentIndex+1, 2, TFT_BLACK);
        }

        m_labels[m_currentIndex].redraw(std::to_string(m_numbers[m_currentIndex]));

        drawRect(m_currentIndex, 1, TFT_YELLOW);
        drawRect(m_currentIndex, 2, TFT_YELLOW);

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

        m_labels[m_currentIndex].redraw(std::to_string(m_numbers[m_currentIndex]));

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
        if (configs.lockscreen.keepLockedAfterReboot.value && configs.lockscreen.locked.value)
        {
            configs.write_config(configs.lockscreen.locked, false);
        }
    }
}

void Lockscreen::buttonPressed(espgui::Button button)
{
    if (configs.lockscreen.allowPresetSwitch.value ||
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

void Lockscreen::drawRect(int index, int offset, uint32_t color) const
{
    espgui::tft.drawRect(m_labels[index].x()-offset, m_labels[index].y()-offset, boxWidth+(offset*2), boxHeight+(offset*2), color);
}
