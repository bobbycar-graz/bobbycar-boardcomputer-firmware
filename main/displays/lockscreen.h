#pragma once

#include <array>

#include "display.h"
#include "widgets/label.h"
#include "globals.h"
#include "utils.h"
#include "texts.h"
#include "modes/ignoreinputmode.h"

namespace {
class MainMenu;
}

namespace {
class Lockscreen : public Display, public DummyBack
{
    using Base = Display;

    static constexpr auto boxWidth = 35;
    static constexpr auto boxHeight = 50;
    static constexpr auto spacing = 20;

public:
    void start() override;
    void initScreen() override;
    void update() override {}
    void redraw() override;
    void stop() override;

    void confirm() override;
    void rotate(int offset) override;

private:
    void drawRect(int index, int offset, uint32_t color) const;

    std::array<Label, 4> m_labels {{
        Label{spacing, 100}, // boxWidth, boxHeight
        Label{spacing*2+boxWidth, 100}, // boxWidth, boxHeight
        Label{spacing*3+boxWidth*2, 100}, // boxWidth, boxHeight
        Label{spacing*4+boxWidth*3, 100} // boxWidth, boxHeight
    }};

    std::array<int8_t, 4> m_numbers;

    uint8_t m_currentIndex{};

    bool m_pressed;
    int m_rotated;

    ModeInterface *m_oldMode;
    IgnoreInputMode m_mode{0, bobbycar::protocol::ControlType::FieldOrientedControl, bobbycar::protocol::ControlMode::Speed};
};

void Lockscreen::start()
{
    m_numbers = {0,0,0,0};
    m_currentIndex = 0;
    m_pressed = false;
    m_rotated = 0;

    m_oldMode = currentMode;
    currentMode = &m_mode;
}

void Lockscreen::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString(TEXT_LOCKVEHICLE, 5, 5);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE);
    tft.drawString("Enter code to unlock:", 0, 50);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    for(int i = 0; i <= 3; i++)
    {
        drawRect(i, 3, TFT_WHITE);
        drawRect(i, 4, TFT_WHITE);
    }

    for (auto &label : m_labels)
        label.start();

    tft.setTextFont(7);

    drawRect(0, 1, TFT_YELLOW);
    drawRect(0, 2, TFT_YELLOW);
    m_labels[0].redraw(std::to_string(m_numbers[0]));
}

void Lockscreen::redraw()
{
    if (m_pressed)
    {
        drawRect(m_currentIndex, 1, TFT_BLACK);
        drawRect(m_currentIndex, 2, TFT_BLACK);

        if (++m_currentIndex>=4)
        {
            if (m_numbers == decltype(m_numbers){1,2,3,4})
            {
                switchScreen<MainMenu>();
                return;
            }

            m_numbers = {0,0,0,0};
            m_currentIndex = 0;
            std::for_each(std::begin(m_labels) + 1, std::end(m_labels), [](auto &label){ label.redraw({}); });
        }

        m_labels[m_currentIndex].redraw(std::to_string(m_numbers[m_currentIndex]));

        drawRect(m_currentIndex, 1, TFT_YELLOW);
        drawRect(m_currentIndex, 2, TFT_YELLOW);

        m_pressed = false;
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
        currentMode = m_oldMode;
}

void Lockscreen::confirm()
{
    m_pressed = true;
}

void Lockscreen::rotate(int offset)
{
    m_rotated += offset;
}

void Lockscreen::drawRect(int index, int offset, uint32_t color) const
{
    tft.drawRect(m_labels[index].x()-offset, m_labels[index].y()-offset, boxWidth+(offset*2), boxHeight+(offset*2), color);
}
}
