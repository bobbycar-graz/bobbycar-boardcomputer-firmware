#include "qrdisplay.h"

// 3rd party libs
#include <tftinstance.h>
#include <screenmanager.h>

using namespace espgui;

QrDisplay::QrDisplay(std::string_view msg, uint8_t ver) :
    m_msg{msg},
    m_ver{ver}
{
}

void QrDisplay::initScreen()
{
    QRCode qrcode;
    uint8_t qrcodeBytes[qrcode_getBufferSize(m_ver)];
    qrcode_initText(&qrcode, qrcodeBytes, m_ver, ECC_MEDIUM, m_msg.data());

    const uint8_t multiplier = (tft.width() - 9) / qrcode.size;
    const uint8_t x_offset = (tft.width() - qrcode.size * multiplier) / 2;
    const uint8_t y_offset = (tft.height() - qrcode.size * multiplier) / 2;

    tft.fillScreen(TFT_WHITE);

    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            if (qrcode_getModule(&qrcode, x, y))
            {
                tft.fillRect(x*multiplier+x_offset,y*multiplier+y_offset, multiplier, multiplier, TFT_BLACK);
            } else {
                tft.fillRect(x*multiplier+x_offset,y*multiplier+y_offset, multiplier, multiplier, TFT_WHITE);
            }
        }
    }
}

void QrDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Left: popScreen(); break;
    default:;
    }
}
