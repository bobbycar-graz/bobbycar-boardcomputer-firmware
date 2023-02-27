#include "setupactions.h"

// 3rdparty lib includes
#include <screenmanager.h>

// local includes
#include "screens/setup/basic_buttons.h"
#include "screens/setup/calibrate_potis.h"

namespace bobby {

PushButtonCalibrateDisplayAction::PushButtonCalibrateDisplayAction(const bool early_return) :
    m_early_return{early_return}
{}

void PushButtonCalibrateDisplayAction::triggered()
{
    espgui::pushScreen<SetupBasicButtonsDisplay>(m_early_return);
}

PushPotiCalibrateDisplayAction::PushPotiCalibrateDisplayAction(const bool early_return) :
        m_early_return{early_return}
{}

void PushPotiCalibrateDisplayAction::triggered()
{
    espgui::pushScreen<SetupCalibratePotisDisplay>(m_early_return);
}
} // namespace bobby
