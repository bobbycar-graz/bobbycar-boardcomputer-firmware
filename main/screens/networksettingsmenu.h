#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class NetworkSettingsMenu : public BobbyMenuDisplay
{
public:
    NetworkSettingsMenu();

    std::string text() const override;
    void back() override;
};

class NetworkAccessPointQRAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

} // namespace bobby
