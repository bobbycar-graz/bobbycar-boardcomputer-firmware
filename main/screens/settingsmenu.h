#pragma once

// local includes
#include "guihelpers/menudisplaywithtime.h"

namespace bobby {

class SettingsMenu : public bobby::MenuDisplayWithTime
{
public:
    SettingsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
