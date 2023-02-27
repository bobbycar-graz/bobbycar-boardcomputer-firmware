#include "bobbyerrorhandler.h"

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbypopupdisplay.h"

namespace bobby {

void BobbyErrorHandler::errorOccurred(std::string &&error)
{
    espgui::changeScreenCallback = [error_ = std::move(error)](espgui::TftInterface &tft){
        auto newDisplay = std::make_unique<BobbyPopupDisplay>(std::string{error_}, std::move(espgui::currentDisplay));
        newDisplay->initOverlay(tft);
        espgui::currentDisplay = std::move(newDisplay);
    };
}
} // namespace bobby
