#pragma once

#include <esp_wifi_types.h>

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "utils.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"
#include "texts.h"

namespace {
template<>
class ChangeValueDisplay<wifi_mode_t> :
    public MenuDisplay,
    public virtual AccessorInterface<wifi_mode_t>,
    public virtual ActionInterface
{
    using Base = MenuDisplay;

public:
    ChangeValueDisplay();

    void start() override;
};

ChangeValueDisplay<wifi_mode_t>::ChangeValueDisplay()
{
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_mode_t>, StaticText<TEXT_WIFI_MODE_NULL>>>(WIFI_MODE_NULL, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_mode_t>, StaticText<TEXT_WIFI_MODE_STA>>>(WIFI_MODE_STA, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_mode_t>, StaticText<TEXT_WIFI_MODE_AP>>>(WIFI_MODE_AP, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_mode_t>, StaticText<TEXT_WIFI_MODE_APSTA>>>(WIFI_MODE_APSTA, *this, *this);
    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&icons::back>>>(*this);
}

void ChangeValueDisplay<wifi_mode_t>::start()
{
    Base::start();

    switch (const auto value = getValue())
    {
    case WIFI_MODE_NULL:  setSelectedIndex(0); break;
    case WIFI_MODE_STA:   setSelectedIndex(1); break;
    case WIFI_MODE_AP:    setSelectedIndex(2); break;
    case WIFI_MODE_APSTA: setSelectedIndex(3); break;
    default:
        //Serial.printf("Unknown wifi_mode_t: %i\r\n", int(value));
        setSelectedIndex(4);
    }
}
}
