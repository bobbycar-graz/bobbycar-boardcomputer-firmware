#pragma once

#include <WiFi.h>

#include "accessorinterface.h"

namespace {
struct WifiModeAccessor : public virtual AccessorInterface<wifi_mode_t>
{
    wifi_mode_t getValue() const override { return WiFi.getMode(); }
    void setValue(wifi_mode_t value) override
    {
        if (!WiFi.mode(value))
            Serial.println("Could not change WiFi mode!");
        // TODO: better error handling
    }
};

struct WifiSleepAccessor : public virtual AccessorInterface<bool>
{
    bool getValue() const override { return WiFi.getSleep(); }
    void setValue(bool value) override
    {
        if (!WiFi.setSleep(value))
            Serial.println("Could not set WiFi sleep!");
        // TODO: better error handling
    }
};

struct WifiTxPowerAccessor : public virtual AccessorInterface<wifi_power_t>
{
    wifi_power_t getValue() const override { return WiFi.getTxPower(); }
    void setValue(wifi_power_t value) override
    {
        if (!WiFi.setTxPower(value))
            Serial.println("Could not set WiFi tx power!");
        // TODO: better error handling
    }
};



struct WifiAutoConnectAccessor : public virtual AccessorInterface<bool>
{
    bool getValue() const override { return WiFi.getAutoConnect(); }
    void setValue(bool value) override
    {
        if (!WiFi.setAutoConnect(value))
            Serial.println("Could not set WiFi autoConnect!");
        // TODO: better error handling
    }
};

struct WifiAutoReconnectAccessor : public virtual AccessorInterface<bool>
{
    bool getValue() const override { return WiFi.getAutoReconnect(); }
    void setValue(bool value) override
    {
        if (!WiFi.setAutoReconnect(value))
            Serial.println("Could not set WiFi autoReconnect!");
        // TODO: better error handling
    }
};
}
