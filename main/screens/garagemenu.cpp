#include "garagemenu.h"

// esp-idf includes
#include <esp_log.h>

// 3rd party libs
#include <actioninterface.h>
#include <menuitem.h>

// local includes
#include "actions/popscreenaction.h"
#include "espnowfunctions.h"
#include "globals.h"
#include "icons/back.h"
#include "newsettings.h"

namespace bobby {

namespace {
constexpr char TEXT_GARAGE[] = "Garage";
constexpr char TEXT_BACK[] = "Back";

class SendEspNowMessageAction : public virtual espgui::ActionInterface
{
public:
    SendEspNowMessageAction(uint8_t index) : m_index{index} {}
    void triggered() override;

private:
    uint8_t m_index;
};
} // namespace

GarageMenu::GarageMenu()
{
    using namespace espgui;

    for (uint8_t index = 0; index < configs.wireless_door_configs.size(); index++)
    {
        const auto &wirelessDoor = configs.wireless_door_configs[index];
        if (wirelessDoor.doorId.value().empty() || wirelessDoor.doorToken.value().empty())
            continue;
        auto &menuitem = constructMenuItem<makeComponentArgs<MenuItem, SendEspNowMessageAction, ChangeableText>>(index);
        menuitem.setTitle(wirelessDoor.doorId.value());
    }

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string GarageMenu::text() const
{
    return TEXT_GARAGE;
}

void GarageMenu::back()
{
    espgui::popScreen();
}

namespace {
void SendEspNowMessageAction::triggered()
{
    if (const auto error = espnow::send_espnow_message(fmt::format("BOBBYOPEN:{}:{}", configs.wireless_door_configs[m_index].doorId.value(), configs.wireless_door_configs[m_index].doorToken.value())); error != ESP_OK)
    {
        ESP_LOGE("BOBBY", "send_espnow_message() failed with: %s", esp_err_to_name(error));
        return;
    }
}
} // namespace

} // namespace bobby
