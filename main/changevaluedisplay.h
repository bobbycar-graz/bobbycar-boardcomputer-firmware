#pragma once

#include "display.h"
#include "textinterface.h"
#include "actioninterface.h"
#include "accessorinterface.h"
#include "widgets/label.h"
#include "globals.h"
#include "utils.h"

namespace {
class ChangeValueDisplayInterface : public Display, public virtual TextInterface, public virtual ActionInterface
{
public:
    void initScreen() override;

    TextInterface *asTextInterface() override { return this; }
    const TextInterface *asTextInterface() const override { return this; }

    ChangeValueDisplayInterface *asChangeValueDisplayInterface() override { return this; }
    const ChangeValueDisplayInterface *asChangeValueDisplayInterface() const override { return this; }

    virtual int shownValue() const = 0;
    virtual void setShownValue(int value) = 0;

protected:
    Label m_titleLabel{5, 5}; // 230, 25
    Label m_valueLabel{26, 81}; // 188, 53
};

template<typename Tvalue>
class ChangeValueDisplaySettingsInterface
{
public:
    virtual Tvalue step() const { return 1; };
};

template<typename Tvalue, typename Tratio>
class RatioNumberStep : public virtual ChangeValueDisplaySettingsInterface<Tvalue>
{
public:
    Tvalue step() const override { return Tvalue(Tratio::num) / Tratio::den; }
};

template<typename Tvalue>
class ChangeValueDisplay : public ChangeValueDisplayInterface, public virtual AccessorInterface<Tvalue>, public virtual ChangeValueDisplaySettingsInterface<Tvalue>
{
    using Base = ChangeValueDisplayInterface;

public:
    void start() override;
    void update() override;
    void redraw() override;

    void rotate(int offset) override;
    void confirm() override;

    int shownValue() const { return m_value; }
    void setShownValue(int value) { m_value = value; }

private:
    Tvalue m_value{};

    int m_rotateOffset;
    bool m_pressed{};
};

void ChangeValueDisplayInterface::initScreen()
{
    tft.fillScreen(TFT_BLACK);

    m_titleLabel.start();

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.drawRect(25, 75, 190, 65, TFT_WHITE);
    m_valueLabel.start();

    tft.setTextFont(4);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Change value and", 10, 160);
    tft.drawString("press button to", 10, 185);
    tft.drawString("confirm and go", 10, 210);
    tft.drawString("back.", 10, 235);
}

template<typename Tvalue>
void ChangeValueDisplay<Tvalue>::start()
{
    m_value = static_cast<AccessorInterface<Tvalue>*>(this)->getValue();

    m_rotateOffset = 0;
    m_pressed = false;
}

template<typename Tvalue>
void ChangeValueDisplay<Tvalue>::update()
{
    if (!m_pressed)
    {
        const auto rotateOffset = m_rotateOffset;
        m_rotateOffset = 0;

        m_value -= rotateOffset * static_cast<ChangeValueDisplaySettingsInterface<Tvalue>*>(this)->step();
    }
    else
    {
        static_cast<AccessorInterface<Tvalue>*>(this)->setValue(m_value);
        triggered();
    }
}

template<typename Tvalue>
void ChangeValueDisplay<Tvalue>::redraw()
{
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);
    m_titleLabel.redraw(text());

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(7);
    m_valueLabel.redraw(std::to_string(m_value));
}

template<typename Tvalue>
void ChangeValueDisplay<Tvalue>::rotate(int offset)
{
    m_rotateOffset += offset;
}

template<typename Tvalue>
void ChangeValueDisplay<Tvalue>::confirm()
{
    m_pressed = true;
}
}

#include "changevaluedisplay_bool.h"
#ifdef FEATURE_BLUETOOTH
#include "changevaluedisplay_bluetoothmode.h"
#endif
#include "changevaluedisplay_controlmode.h"
#include "changevaluedisplay_controltype.h"
#include "changevaluedisplay_larsmmode_mode.h"
#include "changevaluedisplay_unifiedmodelmode.h"
#include "changevaluedisplay_wifi_mode_t.h"
#include "changevaluedisplay_wifi_power_t.h"
