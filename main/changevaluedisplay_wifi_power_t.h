#pragma once

#include "changevaluedisplay.h"
#include "menudisplay.h"
#include "utils.h"
#include "actions/setvalueaction.h"
#include "actions/backproxyaction.h"
#include "icons/back.h"
#include "texts.h"

namespace {
//template<>
//class ChangeValueDisplay<wifi_power_t> :
//    public MenuDisplay,
//    public virtual AccessorInterface<wifi_power_t>,
//    public virtual ActionInterface
//{
//    using Base = MenuDisplay;

//public:
//    ChangeValueDisplay();

//    void start() override;
//};

//ChangeValueDisplay<wifi_power_t>::ChangeValueDisplay()
//{
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_19_5dBm>>>(WIFI_POWER_19_5dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_19dBm>>>(WIFI_POWER_19dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_18_5dBm>>>(WIFI_POWER_18_5dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_17dBm>>>(WIFI_POWER_17dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_15dBm>>>(WIFI_POWER_15dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_13dBm>>>(WIFI_POWER_13dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_11dBm>>>(WIFI_POWER_11dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_8_5dBm>>>(WIFI_POWER_8_5dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_7dBm>>>(WIFI_POWER_7dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_5dBm>>>(WIFI_POWER_5dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_2dBm>>>(WIFI_POWER_2dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, SetValueAction<wifi_power_t>, StaticText<TEXT_WIFI_POWER_MINUS_1dBm>>>(WIFI_POWER_MINUS_1dBm, *this, *this);
//    constructMenuItem<makeComponentArgs<MenuItem, BackProxyAction, StaticText<TEXT_BACK>, StaticMenuItemIcon<&icons::back>>>(*this);
//}

//void ChangeValueDisplay<wifi_power_t>::start()
//{
//    Base::start();

//    switch (const auto value = getValue())
//    {
//    case WIFI_POWER_19_5dBm:    setSelectedIndex(0); break;
//    case WIFI_POWER_19dBm:      setSelectedIndex(1); break;
//    case WIFI_POWER_18_5dBm:    setSelectedIndex(2); break;
//    case WIFI_POWER_17dBm:      setSelectedIndex(3); break;
//    case WIFI_POWER_15dBm:      setSelectedIndex(4); break;
//    case WIFI_POWER_13dBm:      setSelectedIndex(5); break;
//    case WIFI_POWER_11dBm:      setSelectedIndex(6); break;
//    case WIFI_POWER_8_5dBm:     setSelectedIndex(7); break;
//    case WIFI_POWER_7dBm:       setSelectedIndex(8); break;
//    case WIFI_POWER_5dBm:       setSelectedIndex(9); break;
//    case WIFI_POWER_2dBm:       setSelectedIndex(10); break;
//    case WIFI_POWER_MINUS_1dBm: setSelectedIndex(11); break;
//    default:
//        //Serial.printf("Unknown wifi_power_t: %i\r\n", int(value));
//        setSelectedIndex(12);
//    }
//}
}
