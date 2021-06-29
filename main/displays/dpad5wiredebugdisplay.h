#pragma once

#include "display.h"
#include "globals.h"
#include "widgets/label.h"
#include "dpad5wire.h"

namespace {
#ifdef FEATURE_DPAD_5WIRESW
class DPad5WireDebugDisplay : public Display, public virtual DummyConfirm, public virtual DummyBack
{
public:
    void initScreen() override;
    void redraw() override;

private:
    Label m_labelRaw{30, 50};
    Label m_labelUp{175, 75};
    Label m_labelDown{175, 100};
    Label m_labelBack{175, 125};
    Label m_labelConfirm{175, 150};
    Label m_labelProfile0{175, 175};
    Label m_labelProfile1{175, 200};
    Label m_labelProfile2{175, 225};
    Label m_labelProfile3{175, 250};
    Label m_labelGas{175, 275};
    Label m_labelBrems{175, 300};
};

void DPad5WireDebugDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString("DPad 5wire debug", 5, 5);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString("UP:", 30, 75);
    tft.drawString("DOWN:", 30, 100);
    tft.drawString("CONFIRM:", 30, 125);
    tft.drawString("BACK:", 30, 150);
    tft.drawString("PROFILE0:", 30, 175);
    tft.drawString("PROFILE1:", 30, 200);
    tft.drawString("PROFILE2:", 30, 225);
    tft.drawString("PROFILE3:", 30, 250);
    tft.drawString("GAS:", 30, 275);
    tft.drawString("BREMS:", 30, 300);

    m_labelRaw.start();
    m_labelUp.start();
    m_labelDown.start();
    m_labelBack.start();
    m_labelConfirm.start();
    m_labelProfile0.start();
    m_labelProfile1.start();
    m_labelProfile2.start();
    m_labelProfile3.start();
    m_labelGas.start();
    m_labelBrems.start();
}

void DPad5WireDebugDisplay::redraw()
{
    m_labelRaw.redraw(std::string{} +
                    (std::get<0>(dpad5wire::lastState) ? '1' : '0') + ' ' +
                    (std::get<1>(dpad5wire::lastState) ? '1' : '0') + ' ' +
                    (std::get<2>(dpad5wire::lastState) ? '1' : '0') + ' ' +
                    (std::get<3>(dpad5wire::lastState) ? '1' : '0') + ' ' +
                    (std::get<4>(dpad5wire::lastState) ? '1' : '0') + ' ' +
                    (std::get<5>(dpad5wire::lastState) ? '1' : '0') + ' ' +
                    (std::get<6>(dpad5wire::lastState) ? '1' : '0') + ' ' +
                    (std::get<7>(dpad5wire::lastState) ? '1' : '0'));

    m_labelUp.redraw(std::get<DPAD_5WIRESW_UP>(dpad5wire::lastState) ? "1" : "0");
    m_labelDown.redraw(std::get<DPAD_5WIRESW_DOWN>(dpad5wire::lastState) ? "1" : "0");
    m_labelBack.redraw(std::get<DPAD_5WIRESW_BACK>(dpad5wire::lastState) ? "1" : "0");
    m_labelConfirm.redraw(std::get<DPAD_5WIRESW_CONFIRM>(dpad5wire::lastState) ? "1" : "0");
    m_labelProfile0.redraw(std::get<DPAD_5WIRESW_PROFILE0>(dpad5wire::lastState) ? "1" : "0");
    m_labelProfile1.redraw(std::get<DPAD_5WIRESW_PROFILE1>(dpad5wire::lastState) ? "1" : "0");
    m_labelProfile2.redraw(std::get<DPAD_5WIRESW_PROFILE2>(dpad5wire::lastState) ? "1" : "0");
    m_labelProfile3.redraw(std::get<DPAD_5WIRESW_PROFILE3>(dpad5wire::lastState) ? "1" : "0");
    m_labelGas.redraw(raw_gas ? std::to_string(*raw_gas) : "?");
    m_labelBrems.redraw(raw_brems ? std::to_string(*raw_brems) : "?");
}
#endif
}
