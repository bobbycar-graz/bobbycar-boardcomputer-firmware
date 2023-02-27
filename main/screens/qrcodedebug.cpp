#include "qrcodedebug.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "globals.h"
#include "newsettings.h"
#include "screenmanager.h"

namespace bobby {

using namespace espgui;

void QrCodeDebugDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    if (m_qrcode)
    {
        auto &qr = *m_qrcode;
        for (uint8_t y = 0; y < qr.size; y++) {
            for (uint8_t x = 0; x < qr.size; x++) {
                if (qrcode_getModule(&qr, x, y))
                {
                    tft.drawPixel(x+2,y+2, espgui::TFT_BLACK);
                } else {
                    tft.drawPixel(x+2,y+2, espgui::TFT_WHITE);
                }
            }
        }

        m_qrcode.reset();
    }
}

void QrCodeDebugDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Left: popScreen(); break;
    case Button::Right:
    {
        uint8_t qrcodeBytes[qrcode_getBufferSize(7)];
        qrcode_initText(&*m_qrcode, qrcodeBytes, 7, ECC_MEDIUM, fmt::format("WIFI:T:WPA;S:{};P:{};", configs.wifiApName.value(), configs.wifiApKey.value()).c_str());
        break;
    }
    default:;
    }
}
} // namespace bobby
