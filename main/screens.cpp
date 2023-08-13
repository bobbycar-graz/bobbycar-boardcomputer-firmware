#include "screens.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "esptexthelpers.h"
#include "globals.h"
#include "icons/logo.h"

using namespace espgui;

Label bootLabel{32, 250};

namespace {
bool disable_screen_flip{false};
}

void initScreen()
{
    // vertical screen
    tft.init();
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextFont(4);
    tft.setRotation(configs.boardcomputerHardware.flipScreen.value() ? 2 : 0);
    tft.setSwapBytes(true);
    tft.pushImage(0, 40, bobbyicons::logo.WIDTH, bobbyicons::logo.HEIGHT, bobbyicons::logo.buffer);
    tft.setSwapBytes(false);
    tft.drawString("Bobbycar-OS", 32, 200);
    tft.drawString("booting...", 32, 225);
    tft.setTextFont(2);
    tft.drawString("last reboot reason:", 32, 275);
    tft.drawString(espcpputils::toString(esp_reset_reason()), 32, 295);
    tft.setTextFont(4);
    bootLabel.start();
}

void updateRotation()
{
    if (tft.getRotation() != (configs.boardcomputerHardware.flipScreen.value() ? 2 : 0))
    {
        tft.setRotation(configs.boardcomputerHardware.flipScreen.value() ? 2 : 0);
        if (currentDisplay)
            currentDisplay->initScreen();
    }
}

void updateDisplay()
{
    if (currentDisplay)
        currentDisplay->update();

    if (changeScreenCallback)
    {
        changeScreenCallback();
        changeScreenCallback = {};
    }

    if (!disable_screen_flip)
        updateRotation();

    if (const int8_t rawButton = rawButtonRequest.load(); rawButton != -1 && currentDisplay)
    {
        currentDisplay->rawButtonPressed(rawButton);
        currentDisplay->rawButtonReleased(rawButton);
        rawButtonRequest = -1;
    }

    if (const int8_t button = buttonRequest.load(); button != -1 && currentDisplay)
    {
        const auto btn = espgui::Button(button);
        currentDisplay->buttonPressed(btn);
        currentDisplay->buttonReleased(btn);
        buttonRequest = -1;
    }

    if (initScreenRequest && currentDisplay)
    {
        currentDisplay->initScreen();
        initScreenRequest = false;
    }
}

void redrawDisplay()
{
    if (currentDisplay)
    {
        currentDisplay->redraw();
    }
}

void disableScreenFlip(bool enable)
{
    disable_screen_flip = enable;
}
