#include "controller.h"

// local includes
#include "globals.h"

Controller::Controller(
#ifdef FEATURE_SERIAL
    HardwareSerial &serial,
#endif
    bool &enableLeft, bool &enableRight, bool &invertLeft, bool &invertRight,
    int16_t &voltageCalib30V, int16_t &voltageCalib50V) :
#ifdef FEATURE_SERIAL
    serial { serial },
#endif
    enableLeft { enableLeft }, enableRight { enableRight }, invertLeft { invertLeft }, invertRight { invertRight },
    voltageCalib30V { voltageCalib30V }, voltageCalib50V { voltageCalib50V }
{
}

float Controller::getCalibratedVoltage() const
{
    float voltage = feedback.batVoltage;
    if (settings.battery.applyCalibration)
    {
        voltage = ((voltage - float(voltageCalib30V)) * (20.f / (float(voltageCalib50V) - float(voltageCalib30V))) + 30.f);
    }
    else
    {
        voltage = voltage / 100.;
    }
    return voltage;
}
