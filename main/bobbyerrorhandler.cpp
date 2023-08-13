#include "bobbyerrorhandler.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "displays/bobbypopupdisplay.h"

void BobbyErrorHandler::errorOccurred(std::string &&error)
{
    espgui::changeScreenCallback = [error_ = std::move(error)](espgui::TftInterface &tft) mutable {
        auto newDisplay = std::make_unique<BobbyPopupDisplay>(std::move(error_), std::move(espgui::currentDisplay));
        newDisplay->initOverlay(tft);
        espgui::currentDisplay = std::move(newDisplay);
    };
}
