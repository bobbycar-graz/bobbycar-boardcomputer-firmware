#pragma once

// local includes
#include "guihelpers/menudisplaywithtime.h"

namespace bobby {

class TimeSettingsMenu : public bobby::MenuDisplayWithTime
{
public:
    TimeSettingsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
