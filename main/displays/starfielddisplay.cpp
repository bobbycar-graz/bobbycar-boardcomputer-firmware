#include "starfielddisplay.h"

// 3rdparty lib includes
#include <randomutils.h>
#include <esprandom.h>
#include <tftinstance.h>
#include <screenmanager.h>

// local includes
#include "globals.h"
#include "displays/menus/demosmenu.h"

StarfieldDisplay::StarfieldDisplay() :
    za(cpputils::randomNumber<uint8_t>(espcpputils::esp_random_device{})),
    zb(cpputils::randomNumber<uint8_t>(espcpputils::esp_random_device{})),
    zc(cpputils::randomNumber<uint8_t>(espcpputils::esp_random_device{})),
    zx(cpputils::randomNumber<uint8_t>(espcpputils::esp_random_device{}))
{
}

void StarfieldDisplay::initScreen()
{
    Base::initScreen();

    espgui::tft.fillScreen(TFT_BLACK);
    espgui::tft.setRotation(1);

    // fastSetup() must be used immediately before fastPixel() to prepare screen
    // It must be called after any other graphics drawing function call if fastPixel()
    // is to be called again
    //tft.fastSetup(); // Prepare plot window range for fast pixel plotting
}

void StarfieldDisplay::redraw()
{
    Base::redraw();

    uint8_t spawnDepthVariation = 255;

    for(int i = 0; i < NSTARS; ++i)
    {
      if (sz[i] <= 1)
      {
        sx[i] = 160 - 120 + cpputils::randomNumber<uint8_t>(espcpputils::esp_random_device{});
        sy[i] = cpputils::randomNumber<uint8_t>(espcpputils::esp_random_device{});
        sz[i] = spawnDepthVariation--;
      }
      else
      {
        int old_screen_x = ((int)sx[i] - 160) * 256 / sz[i] + 160;
        int old_screen_y = ((int)sy[i] - 120) * 256 / sz[i] + 120;

        // This is a faster pixel drawing function for occassions where many single pixels must be drawn
        espgui::tft.drawPixel(old_screen_x, old_screen_y,TFT_BLACK);

        sz[i] -= 2;
        if (sz[i] > 1)
        {
          int screen_x = ((int)sx[i] - 160) * 256 / sz[i] + 160;
          int screen_y = ((int)sy[i] - 120) * 256 / sz[i] + 120;

          if (screen_x >= 0 && screen_y >= 0 && screen_x < 320 && screen_y < 240)
          {
            uint8_t r, g, b;
            r = g = b = 255 - sz[i];
            espgui::tft.drawPixel(screen_x, screen_y, color565(r,g,b));
          }
          else
            sz[i] = 0; // Out of screen, die.
        }
      }
    }
}

void StarfieldDisplay::stop()
{
    Base::stop();

    espgui::tft.setRotation(0);
}

void StarfieldDisplay::buttonPressed(espgui::Button button)
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
