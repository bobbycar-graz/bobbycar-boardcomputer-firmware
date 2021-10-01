#pragma once

#include "actioninterface.h"
#include "utils.h"
#include "globals.h"
#include <stdio.h>

using namespace espgui;

namespace {
class SelectComrServerAction : public virtual ActionInterface
{
public:
    void triggered() override
    {
        char buffer[70];
        snprintf(buffer, 70, COMR_BUILDURL_TEMPLATE, OTA_USERNAME);
        stringSettings.otaUrl = buffer;
        saveSettings();
    }
};
}
