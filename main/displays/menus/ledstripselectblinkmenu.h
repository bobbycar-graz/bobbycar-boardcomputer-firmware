#pragma once

// Local includes
#include "menudisplay.h"
#include "texts.h"
#include "hardware/ledstrip.h"
#include "helpers/ledstripdefines.h"

#ifdef FEATURE_LEDSTRIP
class currentSelectedBlinkAnimationText : public virtual espgui::TextInterface
{
public: std::string text() const override;
};

class LedstripSelectBlinkMenu :
        public espgui::MenuDisplay,
        public espgui::StaticText<TEXT_BLINKANIMATION>
{
public:
    LedstripSelectBlinkMenu();
    void back() override;
};
#endif
