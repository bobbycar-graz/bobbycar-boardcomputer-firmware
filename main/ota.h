#pragma once

#ifdef FEATURE_ARDUINOOTA
#include <ArduinoOTA.h>
#endif

#include "screens.h"
#include "globals.h"
#include "displays/updatedisplay.h"

namespace {
#ifdef FEATURE_ARDUINOOTA
void initOta()
{
    ArduinoOTA
        .onStart([]() {
            std::string type;
            switch (ArduinoOTA.getCommand())
            {
            case U_FLASH:  type = "sketch"; break;
            case U_SPIFFS: type = "filesystem"; break;
            default: type = "unknown";
            }
            switchScreenImpl<UpdateDisplay>("Updating " + type);
        })
        .onEnd([]() {
            ((UpdateDisplay*)currentDisplay.get())->m_finished = true;
            ((UpdateDisplay*)currentDisplay.get())->redraw();
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            ((UpdateDisplay*)currentDisplay.get())->m_progress = progress;
            ((UpdateDisplay*)currentDisplay.get())->m_total = total;
            ((UpdateDisplay*)currentDisplay.get())->redraw();
        })
        .onError([](ota_error_t error) {
            ((UpdateDisplay*)currentDisplay.get())->m_error = error;
            ((UpdateDisplay*)currentDisplay.get())->m_errorValid = true;
            ((UpdateDisplay*)currentDisplay.get())->redraw();
        });

    ArduinoOTA.begin();
}

void handleOta()
{
    ArduinoOTA.handle();
}
#endif
}
