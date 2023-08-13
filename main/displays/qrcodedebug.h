#pragma once

// 3rdparty lib includes
#include <qrcode.h>

// local includes
#include "bobbydisplay.h"

class QrCodeDebugDisplay :
    public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    QrCodeDebugDisplay();

    void initScreen(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

private:
    QRCode m_qrcode;
};
