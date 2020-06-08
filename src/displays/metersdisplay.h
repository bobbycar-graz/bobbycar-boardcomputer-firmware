#pragma once

#include <array>

#include <Arduino.h>

#include "demodisplay.h"
#include "actions/switchscreenaction.h"
#include "globals.h"
#include "utils.h"

namespace {
class MainMenu;
class StatusDisplay;
class BmsDisplay;
}

namespace {
class MetersDisplay : public DemoDisplay, public SwitchScreenAction<MainMenu>, public BackActionInterface<SwitchScreenAction<MainMenu>>
{
    using Base = DemoDisplay;

public:
    void initScreen() override;
    void redraw() override;

    void rotate(int offset) override;

private: 
    class BoardStatus
    {
        public:
        void redraw(const Controller &controller);
        int batv;
        int temper;
    };
    //  Draw the analogue meter on the screen
    void analogMeter();

    // Update needle position
    void plotNeedle(float value);

    //  Draw a linear meter on the screen
    void plotLinear(const char *label, int x, int y);

    //  Adjust 6 linear meter pointer positions
    void plotPointer();

    static const constexpr auto TFT_GREY = 0x5AEB;

    float ltx = 0;    // Saved x coord of bottom of needle
    uint16_t osx = 120, osy = 120; // Saved x & y coords

    struct ValuePair {
        int value = 0,
            old_value = -1;
    };
    std::array<ValuePair, 6> values;

    int d = 0;
};

void MetersDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);

    analogMeter(); // Draw analogue meter

    // Draw 6 linear meters
    byte d = 40;
    plotLinear("Gas", 0, 160);
    plotLinear("Brems", 1 * d, 160);
    plotLinear("Bat %", 2 * d, 160);
    plotLinear("Amp", 3 * d, 160);
    plotLinear("T", 4 * d, 160);
    plotLinear("speed", 5 * d, 160);
}
void MetersDisplay::redraw()
{
    d += 4; if (d >= 360) d = 0;

    // Create a Sine wave for testing
    //for (auto iter = std::begin(values); iter != std::end(values); iter++)
    auto iter = std::begin(values);
    iter->value = {(int)gas/10};
    iter++;
    iter->value = {(int)brems/10};
    iter++;
    if(front.feedbackValid)
    {
        iter->value = {(int)((front.feedback.batVoltage/100)-36)*7};
        iter++;
        iter->value = {(int)sumCurrent};
        iter++;
        iter->value = {(int)front.feedback.boardTemp/10};
        iter++;
        iter->value = {(int)(front.feedback.left.speed-front.feedback.right.speed)/15};
    }
    else
    {
        iter->value = {(int)0};
        iter++;
        iter->value = {(int)0};
    }
    
    plotPointer();
    

    plotNeedle(avgSpeedKmh);
}
void MetersDisplay::BoardStatus::redraw(const Controller &controller)
{
    if(controller.feedbackValid)
    {
        batv = (int) controller.feedback.batVoltage;
        temper = (int) controller.feedback.boardTemp;
    }
}
void MetersDisplay::rotate(int offset)
{
    if (offset < 0)
        switchScreen<StatusDisplay>();
    else if (offset > 0)
#ifdef FEATURE_BMS
        switchScreen<BmsDisplay>();
#else
        switchScreen<StatusDisplay>();
#endif
}

void MetersDisplay::analogMeter()
{
    // Meter outline
    tft.fillRect(0, 0, 239, 126, TFT_GREY);
    tft.fillRect(5, 3, 230, 119, TFT_WHITE);

    tft.setTextColor(TFT_BLACK);    // Text colour

    // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
    for (int i = -50; i < 51; i += 5) {
        // Long scale tick length
        int tl = 15;

        // Coodinates of tick to draw
        float sx = cos((i - 90) * 0.0174532925);
        float sy = sin((i - 90) * 0.0174532925);
        uint16_t x0 = sx * (100 + tl) + 120;
        uint16_t y0 = sy * (100 + tl) + 140;
        uint16_t x1 = sx * 100 + 120;
        uint16_t y1 = sy * 100 + 140;

        // Coordinates of next tick for zone fill
        float sx2 = cos((i + 5 - 90) * 0.0174532925);
        float sy2 = sin((i + 5 - 90) * 0.0174532925);
        int x2 = sx2 * (100 + tl) + 120;
        int y2 = sy2 * (100 + tl) + 140;
        int x3 = sx2 * 100 + 120;
        int y3 = sy2 * 100 + 140;

        // Yellow zone limits
        //if (i >= -50 && i < 0) {
        //    tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
        //    tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
        //}

        // Green zone limits
        if (i >= 0 && i < 25) {
            tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN);
            tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);
        }

        // Orange zone limits
        if (i >= 25 && i < 50) {
            tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_ORANGE);
            tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_ORANGE);
        }

        // Short scale tick length
        if (i % 25 != 0) tl = 8;

        // Recalculate coords incase tick lenght changed
        x0 = sx * (100 + tl) + 120;
        y0 = sy * (100 + tl) + 140;
        x1 = sx * 100 + 120;
        y1 = sy * 100 + 140;

        // Draw tick
        tft.drawLine(x0, y0, x1, y1, TFT_BLACK);

        // Check if labels should be drawn, with position tweaks
        if (i % 25 == 0) {
            // Calculate label positions
            x0 = sx * (100 + tl + 10) + 120;
            y0 = sy * (100 + tl + 10) + 140;
            switch (i / 25) {
                case -2: tft.drawCentreString("0", x0, y0 - 12, 2); break;
                case -1: tft.drawCentreString("7.5", x0, y0 - 9, 2); break;
                case 0: tft.drawCentreString("15", x0, y0 - 6, 2); break;
                case 1: tft.drawCentreString("22.5", x0, y0 - 9, 2); break;
                case 2: tft.drawCentreString("30", x0, y0 - 12, 2); break;
            }
        }

        // Now draw the arc of the scale
        sx = cos((i + 5 - 90) * 0.0174532925);
        sy = sin((i + 5 - 90) * 0.0174532925);
        x0 = sx * 100 + 120;
        y0 = sy * 100 + 140;
        // Draw scale arc, don't draw the last part
        if (i < 50) tft.drawLine(x0, y0, x1, y1, TFT_BLACK);
    }

    tft.drawString("KM/h", 5 + 230 - 40, 119 - 20, 2); // Units at bottom right
    tft.drawCentreString("KM/h", 120, 70, 4); // Comment out to avoid font 4
    tft.drawRect(5, 3, 230, 119, TFT_BLACK); // Draw bezel line

    plotNeedle(0.f); // Put meter needle at 0
}

void MetersDisplay::plotNeedle(float value)
{
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    char buf[8]; dtostrf(value, 4, 0, buf);
    tft.drawRightString(buf, 50, 119 - 25, 4);

    if (value < -3) value = -3; // Limit value to emulate needle end stops
    if (value > 33) value = 33;

    float sdeg = map(value, -3, 33, -150, -30); // Map value to angle
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, TFT_WHITE);

    // Re-plot text under needle
    tft.setTextColor(TFT_BLACK);
    tft.drawCentreString("KM/h", 120, 70, 4); // // Comment out to avoid font 4

    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 98 + 120;
    osy = sy * 98 + 140;

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, TFT_RED);
    tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, TFT_MAGENTA);
    tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, TFT_RED);
}

void MetersDisplay::plotLinear(const char *label, int x, int y)
{
    int w = 36;
    tft.drawRect(x, y, w, 155, TFT_GREY);
    tft.fillRect(x + 2, y + 19, w - 3, 155 - 38, TFT_WHITE);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawCentreString(label, x + w / 2, y + 2, 2);

    for (int i = 0; i < 110; i += 10)
    {
        tft.drawFastHLine(x + 20, y + 27 + i, 6, TFT_BLACK);
    }

    for (int i = 0; i < 110; i += 50)
    {
        tft.drawFastHLine(x + 20, y + 27 + i, 9, TFT_BLACK);
    }

    tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 - 5, TFT_RED);
    tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 + 5, TFT_RED);

    tft.drawCentreString("---", x + w / 2, y + 155 - 18, 2);
}

void MetersDisplay::plotPointer()
{
    int dy = 187;
    byte pw = 16;

    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    // Move the 6 pointers one pixel towards new value
    for (auto iter = std::begin(values); iter != std::end(values); iter++)
    {
        const auto i = std::distance(std::begin(values), iter);

        char buf[8];
        dtostrf(iter->value, 4, 0, buf);
        tft.drawRightString(buf, i * 40 + 36 - 5, 187 - 27 + 155 - 18, 2);

        int dx = 3 + 40 * i;
        if (iter->value < 0) iter->value = 0; // Limit value to emulate needle end stops
        if (iter->value > 100) iter->value = 100;

        while (!(iter->value == iter->old_value)) {
            dy = 187 + 100 - iter->old_value;
            if (iter->old_value > iter->value)
            {
                tft.drawLine(dx, dy - 5, dx + pw, dy, TFT_WHITE);
                iter->old_value--;
                tft.drawLine(dx, dy + 6, dx + pw, dy + 1, TFT_RED);
            }
            else
            {
                tft.drawLine(dx, dy + 5, dx + pw, dy, TFT_WHITE);
                iter->old_value++;
                tft.drawLine(dx, dy - 6, dx + pw, dy - 1, TFT_RED);
            }
        }
    }
}
}
