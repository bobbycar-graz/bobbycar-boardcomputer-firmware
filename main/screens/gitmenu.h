#pragma once

// local includes
#include "guihelpers/menudisplaywithtime.h"

namespace bobby {

class GitMenu : public bobby::MenuDisplayWithTime
{
public:
    GitMenu();

    std::string text() const override;

    void back() override;
};

} // namespace bobby
