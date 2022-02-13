#include "featureflagsmenu.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <actions/switchscreenaction.h>
#include <icons/back.h>
#include <strutils.h>

// local includes
#include "displays/bobbypopupdisplay.h"
#include "displays/menus/settingsmenu.h"
#include "bobbycheckbox.h"
#include "newsettings.h"

using namespace espgui;

namespace {
constexpr const char * const TAG = "FEATUREFLAGSMENU";

constexpr char TEXT_FEATUREFLAGS[] = "Feature Flags";
constexpr char TEXT_BACK[] = "Back";

class FeatureFlagMenuItem : public MenuItem, public virtual BobbyErrorHandler {
public:
    explicit FeatureFlagMenuItem(ConfigWrapper<bool> &config) : m_config{config} {}
    std::string text() const override
    {
        std::string_view name = m_config.nvsName();
        if (cpputils::stringStartsWith(name, "f_")) {
            name.remove_prefix(std::strlen("f_"));
        }
        return std::string{name};
    }

    void triggered() override
    {
        if (auto result = m_config.write(configs.nvs_handle_user, !m_config.value); !result)
            errorOccured(std::move(result).error());
    }

    const MenuItemIcon *icon() const override
    {
        return m_config.value ? &icons::checked : &icons::unchecked;
    }
private:
    ConfigWrapper<bool> &m_config;
};

// TODO: Replace SwitchScreenAction / switchScreen with this action. Needs: BobbyPopupDisplayWithCustomExitAction => pass SwitchScreenAction<SettingsMenu> into it

class ExitFeatureFlagsMenuAction : public virtual ActionInterface {
public:
    void triggered() override {
        auto newDisplay = std::make_unique<BobbyPopupDisplay>(std::move("Please reboot if you have changed something"), std::move(espgui::currentDisplay));
        newDisplay->initOverlay();
        espgui::currentDisplay = std::move(newDisplay);
    }
};

} // namespace

FeatureFlagsMenu::FeatureFlagsMenu()
{
    configs.callForEveryFeature([&](ConfigWrapper<bool> &feature){
        constructMenuItem<FeatureFlagMenuItem>(feature);
    });
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<SettingsMenu>>>();
}

std::string FeatureFlagsMenu::text() const
{
    return TEXT_FEATUREFLAGS;
}

void FeatureFlagsMenu::back()
{
    switchScreen<SettingsMenu>();
}
