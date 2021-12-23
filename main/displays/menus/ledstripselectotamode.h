#pragma once

// Local includes
#include "menudisplay.h"
#include "texts.h"
#include "hardware/ledstrip.h"
#include "globals.h"
#include "utils.h"

#if defined(FEATURE_LEDSTRIP) && defined(FEATURE_OTA)

template <OtaAnimationModes mode>
class LedstripChangeOtaAnimModeAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        settings.ledstrip.otaMode = mode;
        saveSettings();
    }
};

class ledstripOtaAnimationChangeMenu :
        public espgui::MenuDisplay,
        public espgui::StaticText<TEXT_BLINKANIMATION>
{
public:
    ledstripOtaAnimationChangeMenu();
    void back() override;
};
#endif
