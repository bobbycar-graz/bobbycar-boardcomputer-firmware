#if defined(FEATURE_GARAGE) && defined (FEATURE_ESPNOW)
#include "garagenmenu.h"

// 3rd party libs
#include <icons/back.h>
#include <menuitem.h>

// local includes
#include "actions/switchscreenaction.h"
#include "displays/menus/mainmenu.h"

using namespace espgui;

GarageMenu::GarageMenu()
{
    for (uint8_t index = 0; index < configs.wireless_door_configs.size(); index++)
    {
        const auto &wirelessDoor = configs.wireless_door_configs[index];
        if (wirelessDoor.doorId.value.empty() || wirelessDoor.doorToken.value.empty())
            continue;
        auto &menuitem = constructMenuItem<makeComponentArgs<MenuItem, garagenmenu::SendEspNowMessageAction, ChangeableText>>(index);
        menuitem.setTitle(wirelessDoor.doorId.value);
    }

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void GarageMenu::back()
{
    switchScreen<MainMenu>();
}
#endif
