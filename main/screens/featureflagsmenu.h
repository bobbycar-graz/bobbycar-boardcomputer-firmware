#pragma once

// local includes
#include "guihelpers/menudisplaywithtime.h"

namespace bobby {

class FeatureFlagsMenu : public bobby::MenuDisplayWithTime
{
    using Base = bobby::MenuDisplayWithTime;
public:
    FeatureFlagsMenu();
    void start() override;

    std::string text() const override;

    void back() override;
};

} // namespace bobby
