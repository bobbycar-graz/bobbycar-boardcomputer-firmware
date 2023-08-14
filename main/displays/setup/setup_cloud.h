#pragma once

// local includes
#include "displays/bobbymenudisplay.h"

class SetupCloudDisplay : public virtual BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;
public:
    explicit SetupCloudDisplay(bool early_return = false);

    void start() override;
    void stop() override;

    void back() override;

    [[nodiscard]] std::string title() const override;
private:
    const bool m_early_return;
};
