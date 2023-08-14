#include "recoverymenu.h"

// 3dparty lib includes
#include <menuitem.h>

// local includes
#include "actions/rebootaction.h"
#include "actions/resetnvsaction.h"
#include "bobbycheckbox.h"
#include "bobbyerrorhandler.h"
#include "icons/info.h"
#include "icons/reboot.h"
#include "newsettings.h"

namespace {
constexpr char TEXT_RESET_NVS[] = "Reset NVS";
constexpr char TEXT_REBOOT[] = "Reboot";
}

class BasicFeatureFlagMenuItem : public espgui::MenuItem
{
public:
    BasicFeatureFlagMenuItem(ConfiguredFeatureFlag &flag) :
        m_flag{flag}
    {}

    std::string text() const override
    {
        return m_flag.isEnabled.nvsName();
    }

    void triggered() override
    {
        if (auto result = m_flag.isEnabled.write(configs.nvs_handle_user, !m_flag.isEnabled.value()); !result)
            BobbyErrorHandler{}.errorOccurred(std::move(result).error());
    }

    const espgui::MenuItemIcon *icon() const override
    {
        return m_flag.isEnabled.value() ? &espgui::icons::checked : &espgui::icons::unchecked;
    }
private:
    ConfiguredFeatureFlag &m_flag;
};

RecoveryMenu::RecoveryMenu()
{
    using namespace espgui;

    configs.callForEveryFeature([&](ConfiguredFeatureFlag &feature){
        constructMenuItem<BasicFeatureFlagMenuItem>(feature);
    });
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_RESET_NVS>, ResetNVSAction<false>, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>, RebootAction, StaticMenuItemIcon<&bobbyicons::reboot>>>();
}

std::string RecoveryMenu::title() const
{
    return "Recovery Menu";
}
