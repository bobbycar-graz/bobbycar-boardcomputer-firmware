#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class NetworkSettingsMenu : public BobbyMenuDisplay
{
public:
    NetworkSettingsMenu();

    std::string title() const override;
    void back() override;
};

class NetworkAccessPointQRAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
