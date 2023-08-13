#include "bobbyerrorhandler.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "displays/bobbypopupdisplay.h"

void BobbyErrorHandler::errorOccurred(std::string &&error)
{
    auto newDisplay = std::make_unique<BobbyPopupDisplay>(std::move(error), std::move(espgui::currentDisplay));
    newDisplay->initOverlay(tft);
    espgui::currentDisplay = std::move(newDisplay);
}
