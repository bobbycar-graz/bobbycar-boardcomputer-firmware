#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

#ifdef FEATURE_OTA
class LedstripOtaAnimationChangeMenu : public BobbyMenuDisplay
{
public:
    LedstripOtaAnimationChangeMenu();

    std::string text() const override;
    void back() override;
};
#endif
