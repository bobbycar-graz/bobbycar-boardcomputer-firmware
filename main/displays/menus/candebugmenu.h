#pragma once

#include "sdkconfig.h"

// local includes
#include "displays/bobbymenudisplay.h"

#ifdef FEATURE_CAN

// system includes
#include <expected>

// esp-idf includes
#include <driver/twai.h>

class CanDebugMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    CanDebugMenu();

    std::string title() const override;

    void update() override;

    void back() override;

private:
    std::expected<twai_status_info_t, esp_err_t> m_last_can_status_info;
};

#endif
