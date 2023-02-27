#pragma once

// system includes
#include <optional>

// 3rdparty lib includes
#include <qrcode.h>

// local includes
#include "guihelpers/bobbydisplay.h"

namespace bobby {

class QrCodeDebugDisplay :
    public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void redraw(espgui::TftInterface &tft) override;

    void buttonPressed(espgui::Button button) override;

private:
    std::optional<QRCode> m_qrcode;
};
} // namespace bobby
