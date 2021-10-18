#include "esptexthelpers.h"

// esp-idf includes
#include <esp_system.h>

// 3rdparty lib includes
#include <espstrutils.h>

std::string LastRebootReasonText::text() const
{
    return fmt::format("Last Reboot Reason: {}", espcpputils::toString(esp_reset_reason()));
}
