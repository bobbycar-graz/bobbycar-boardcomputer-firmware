#include "spirodisplay.h"

// 3rdparty lib includes
#include <randomutils.h>
#include <esprandom.h>
#include <tftinstance.h>
#include <screenmanager.h>

void SpiroDisplay::initScreen()
{
    Base::initScreen();

    espgui::tft.setRotation(3);
}

void SpiroDisplay::redraw()
{
    Base::redraw();

    for (int j = 0; j < std::max(1, n); j++)
    {
        if (i == 0)
        {
            espgui::tft.fillScreen(TFT_BLACK);
            n = cpputils::randomNumber<uint8_t>(2, 23, espcpputils::esp_random_device{});
            r = cpputils::randomNumber<uint8_t>(20, 100, espcpputils::esp_random_device{});
            colour = 0; //rainbow();
        }

        if (i < (360 * n))
        {
            sx = std::cos((i / n - 90) * DEG2RAD);
            sy = std::sin((i / n - 90) * DEG2RAD);
            x0 = sx * (120 - r) + 159;
            yy0 = sy * (120 - r) + 119;


            sy = std::cos(((i % 360) - 90) * DEG2RAD);
            sx = std::sin(((i % 360) - 90) * DEG2RAD);
            x1 = sx * r + x0;
            yy1 = sy * r + yy0;
            espgui::tft.drawPixel(x1, yy1, rainbow(map(i%360,0,360,0,127))); //colour);
        }

        if (i == (360 * n))
        {
            r = cpputils::randomNumber<uint8_t>(20, 100, espcpputils::esp_random_device{});//r = r / random(2,4);
        }

        if (i >= (360 * n))
        {
            auto new_i = i - (360 * n);

            sx = std::cos((new_i / n - 90) * DEG2RAD);
            sy = std::sin((new_i / n - 90) * DEG2RAD);
            x0 = sx * (120 - r) + 159;
            yy0 = sy * (120 - r) + 119;


            sy = std::cos(((new_i % 360) - 90) * DEG2RAD);
            sx = std::sin(((new_i % 360) - 90) * DEG2RAD);
            x1 = sx * r + x0;
            yy1 = sy * r + yy0;
            espgui::tft.drawPixel(x1, yy1, rainbow(map(new_i%360,0,360,0,127))); //colour);
        }

        i++;
        if (i == 2* (360 * n))
            i = 0;
    }
}

void SpiroDisplay::stop()
{
    Base::stop();

    espgui::tft.setRotation(0);
}

void SpiroDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
        using espgui::Button;
    case Button::Left:
        espgui::popScreen();
        break;
    default:;
    }
}

unsigned int SpiroDisplay::rainbow(int value)
{
    // Value is expected to be in range 0-127
    // The value is converted to a spectrum colour from 0 = blue through to red = blue
    //int value = random (128);
    byte red = 0; // Red is the top 5 bits of a 16 bit colour value
    byte green = 0;// Green is the middle 6 bits
    byte blue = 0; // Blue is the bottom 5 bits

    byte quadrant = value / 32;

    if (quadrant == 0) {
        blue = 31;
        green = 2 * (value % 32);
        red = 0;
    }
    if (quadrant == 1) {
        blue = 31 - (value % 32);
        green = 63;
        red = 0;
    }
    if (quadrant == 2) {
        blue = 0;
        green = 63;
        red = value % 32;
    }
    if (quadrant == 3) {
        blue = 0;
        green = 63 - 2 * (value % 32);
        red = 31;
    }
    return (red << 11) + (green << 5) + blue;
}
