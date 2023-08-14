#pragma once

// esp-idf includes
#include <esp_wifi_types.h>

// local includes
#include "displays/bobbymenudisplay.h"

class WifiStaScanEntryMenu : public BobbyMenuDisplay
{
public:
    WifiStaScanEntryMenu(const wifi_ap_record_t &info);

    std::string title() const override;
    void back() override;

private:
    const wifi_ap_record_t m_info;
};
