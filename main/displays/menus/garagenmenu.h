#pragma once

#ifdef FEATURE_ESPNOW
// local includes
#include "displays/bobbymenudisplay.h"

class GarageMenu : public BobbyMenuDisplay
{
public:
    GarageMenu();

    std::string text() const override;

    void back() override;
};
#endif
