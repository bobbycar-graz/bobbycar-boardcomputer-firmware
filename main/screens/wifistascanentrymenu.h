#pragma once

// esp-idf includes
#include "esp_wifi_types.h"

// local includes
#include "guihelpers/bobbymenudisplay.h"

namespace bobby {

class WifiStaScanEntryMenu : public BobbyMenuDisplay
{
public:
    WifiStaScanEntryMenu(const wifi_ap_record_t &info);

    std::string text() const override;
    void back() override;

private:
    const wifi_ap_record_t m_info;
};
} // namespace bobby
