#include "featureflagsmenu.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <actions/switchscreenaction.h>
#include <icons/back.h>

// local includes
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
    std::string text() const override { return m_config.nvsName(); }

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
