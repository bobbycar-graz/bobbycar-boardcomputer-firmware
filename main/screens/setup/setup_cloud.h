#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class SetupCloudDisplay : public virtual BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    explicit SetupCloudDisplay(bool early_return = false);

    void start() override;
    void stop() override;

    void back() override;

    [[nodiscard]] std::string text() const override;
private:
    const bool m_early_return;
};
} // namespace bobby
