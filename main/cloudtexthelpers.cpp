#include "cloudtexthelpers.h"

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "cloud.h"

std::string CloudCreatedText::text() const
{
    return fmt::format("created: {}", cloudClient ? "true" : "false");
}

std::string CloudStartedText::text() const
{
    std::string text = "started: ";
    if (cloudClient)
        text += cloudStarted ? "true" : "false";
    return text;
}

std::string CloudConnectedText::text() const
{
    std::string text = "connected: ";
    if (cloudClient)
        text += cloudClient.is_connected() ? "true" : "false";
    return text;
}
