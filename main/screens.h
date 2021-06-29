#pragma once

#include <optional>

#include "globals.h"
#include "utils.h"
#include "widgets/label.h"
#include "icons/logo.h"

namespace {
Label bootLabel{32, 250};

void deconstructScreen()
{
    if (currentDisplay)
    {
        currentDisplay->stop();
        //currentDisplay->~Display();
        currentDisplay = nullptr;
    }
}

template<typename T, typename... Args>
void switchScreenImpl(Args&&... args)
{
    deconstructScreen();

    std::unique_ptr<T> ptr = std::make_unique<T>(std::forward<Args>(args)...);
    currentDisplay = std::move(ptr);
    currentDisplay->start();
    currentDisplay->initScreen();
    currentDisplay->update();
    currentDisplay->redraw();
}

std::function<void()> changeScreenCallback;

template<typename T, typename... Args>
void switchScreen(Args&&... args)
{
    if (currentDisplay)
        changeScreenCallback = [&args...](){ switchScreenImpl<T>(std::forward<Args>(args)...); };
    else
        switchScreenImpl<T>(std::forward<Args>(args)...);
}

void initScreen()
{
    tft.init();
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextFont(4);
    //tft.pushImage(0, 40, icons::logo.WIDTH, icons::logo.HEIGHT, icons::logo.buffer);
    tft.drawString("Bobbycar-OS", 32, 200);
    tft.drawString("booting...", 32, 225);
    bootLabel.start();
}

void updateDisplay()
{
    if (rotated)
    {
        const auto rotatedCopy = rotated;
        rotated = 0;

        if (currentDisplay)
            currentDisplay->rotate(rotatedCopy);
    }

    if (requestFullRedraw)
    {
        requestFullRedraw = false;

        tft.init();

        if (currentDisplay)
            currentDisplay->initScreen();
    }

    if (confirmButtonPressed)
    {
        confirmButtonPressed = false;

        if (currentDisplay)
            currentDisplay->confirm();
    }

    if (confirmButtonLongPressed)
    {
        confirmButtonLongPressed = false;
        //Serial.println("todo: implement long press for confirm");
    }

    if (backButtonPressed)
    {
        backButtonPressed = false;

        if (currentDisplay)
            currentDisplay->back();
    }

    if (backButtonLongPressed)
    {
        backButtonLongPressed = false;
        //Serial.println("todo: implement long press for back");
    }

    if (currentDisplay)
        currentDisplay->update();

    if (changeScreenCallback)
    {
        changeScreenCallback();
        changeScreenCallback = {};
    }
}

void redrawDisplay()
{
    if (currentDisplay)
    {
        tft.setSwapBytes(settings.boardcomputerHardware.swapScreenBytes);
        currentDisplay->redraw();
    }
}
}
