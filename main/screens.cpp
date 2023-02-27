#include "screens.h"

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftespiimpl.h>
#include <tftinterface.h>

// local includes
#include "globals.h"
#include "icons/logo.h"
#include "texthelpers/esptexthelpers.h"

namespace bobby {

using namespace espgui;

espgui::Label bootLabel{32, 250};

namespace {
bool disable_screen_flip{false};

espgui::TftESpiImpl tft;
}

void tft_init()
{
    tft.init();
}

void initScreen()
{
    // vertical screen
    tft.init();
    tft.fillScreen(espgui::TFT_WHITE);
    tft.setRotation(configs.boardcomputerHardware.flipScreen.value() ? 2 : 0);
    tft.setSwapBytes(true);
    tft.pushImage(0, 40, bobbyicons::logo.WIDTH, bobbyicons::logo.HEIGHT, bobbyicons::logo.buffer);
    tft.setSwapBytes(false);
    tft.drawString("Bobbycar-OS", 32, 200, espgui::TFT_BLACK, espgui::TFT_WHITE, 4);
    tft.drawString("booting...", 32, 225, espgui::TFT_BLACK, espgui::TFT_WHITE, 4);
    tft.drawString("last reboot reason:", 32, 275, espgui::TFT_BLACK, espgui::TFT_WHITE, 2);
    tft.drawString(espcpputils::toString(esp_reset_reason()), 32, 295, espgui::TFT_BLACK, espgui::TFT_WHITE, 2);
    bootLabel.start(tft);
}

void updateRotation()
{
    if (tft.getRotation() != (configs.boardcomputerHardware.flipScreen.value() ? 2 : 0))
    {
        tft.setRotation(configs.boardcomputerHardware.flipScreen.value() ? 2 : 0);
        if (currentDisplay)
            currentDisplay->initScreen(tft);
    }
}

void updateDisplay()
{
    if (currentDisplay)
        currentDisplay->update();

    if (changeScreenCallback)
    {
        changeScreenCallback(tft);
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
        currentDisplay->initScreen(tft);
        initScreenRequest = false;
    }
}

void redrawDisplay()
{
    if (currentDisplay)
    {
        currentDisplay->redraw(tft);
    }
}

void disableScreenFlip(bool enable)
{
    disable_screen_flip = enable;
}

void set_boot_msg(std::string_view msg)
{
    bootLabel.redraw(tft, msg, espgui::TFT_BLACK, espgui::TFT_WHITE, 4);
}

uint16_t getScreenWidth()
{
    return tft.width();
}

uint16_t getScreenHeight()
{
    return tft.height();
}
} // namespace bobby
