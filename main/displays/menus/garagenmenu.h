#pragma once
#if defined(FEATURE_GARAGE) && defined (FEATURE_ESPNOW)
// 3rd party lib
#include <actioninterface.h>
#include <esp_log.h>
#include <menuitem.h>

// local includes
#include "displays/bobbymenudisplay.h"
#include "espnowfunctions.h"
#include "texts.h"
#include "globals.h"

namespace garagenmenu {
class SendEspNowMessageAction : public virtual espgui::ActionInterface
{
public:
    SendEspNowMessageAction(uint8_t index) : m_index{index} {}
    void triggered() override
    {

    }
private:
    uint8_t m_index;
};
} // namespace garagenmenu

class GarageMenu :
        public BobbyMenuDisplay,
        public espgui::StaticText<TEXT_GARAGE>
{
public:
    GarageMenu();
    void back() override;
};
#endif
