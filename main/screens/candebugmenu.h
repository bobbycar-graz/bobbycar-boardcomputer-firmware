#pragma once

// local includes
#include "guihelpers/bobbymenudisplay.h"

#ifdef FEATURE_CAN

// esp-idf includes
#include "driver/twai.h"

// 3rdparty lib includes
#include "tl/expected.hpp"

namespace bobby {

class CanDebugMenu : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    CanDebugMenu();

    std::string text() const override;

    void update() override;

    void back() override;

private:
    tl::expected<twai_status_info_t, esp_err_t> m_last_can_status_info;
};

} // namespace bobby

#endif
