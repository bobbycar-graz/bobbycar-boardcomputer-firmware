#pragma once

// Local includes
#include "guihelpers/menudisplaywithtime.h"

namespace bobby {

class StatisticsMenu : public bobby::MenuDisplayWithTime
{
public:
    StatisticsMenu();

    std::string text() const override;

    void back() override;
};
} // namespace bobby
