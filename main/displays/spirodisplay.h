#pragma once

#include <cstdint>

#include "display.h"
#include "actions/switchscreenaction.h"

namespace {
class DemosMenu;
}

namespace {
class SpiroDisplay : public Display, public SwitchScreenAction<DemosMenu>, public DummyConfirm, public BackActionInterface<SwitchScreenAction<DemosMenu>>
{
public:
    void initScreen() override;
    void redraw() override;
    void stop() override;

private:
    constexpr static auto DEG2RAD = 0.0174532925;

    static unsigned int rainbow(int value);

    float sx = 0, sy = 0;
    uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;

    long i{0};
    int n{}, r{}, colour{};
};

void SpiroDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(3);
}

void SpiroDisplay::redraw()
{
    for (int j = 0; j < std::max(1, n); j++)
    {
        if (i == 0)
        {
            tft.fillScreen(TFT_BLACK);
            n = random(2, 23);
            r = random(20, 100);
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
            tft.drawPixel(x1, yy1, rainbow(map(i%360,0,360,0,127))); //colour);
        }

        if (i == (360 * n))
        {
            r = random(20, 100);//r = r / random(2,4);
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
            tft.drawPixel(x1, yy1, rainbow(map(new_i%360,0,360,0,127))); //colour);
        }

        i++;
        if (i == 2* (360 * n))
            i = 0;
    }
}

void SpiroDisplay::stop()
{
    tft.setRotation(0);
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
}
