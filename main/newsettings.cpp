#include "newsettings.h"

// 3rdparty lib includes
#include <configmanager_priv.h>

std::string defaultHostname()
{
    return "hallo";
}

ConfigManager<ConfigContainer> configs;

INSTANTIATE_CONFIGMANAGER_TEMPLATES(ConfigContainer)
