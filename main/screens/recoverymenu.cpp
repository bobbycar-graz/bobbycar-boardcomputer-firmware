#include "recoverymenu.h"

// 3rdparty lib includes
#include <actions/pushscreenaction.h>
#include <menuitem.h>

// local includes
#include "actions/resetnvsaction.h"
#include "guihelpers/bobbycheckbox.h"
#include "guihelpers/bobbyerrorhandler.h"
#include "icons/checked.h"
#include "icons/info.h"
#include "icons/reboot.h"
#include "icons/unchecked.h"
#include "newsettings.h"
#include "screens/rebootscreen.h"

namespace bobby {

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
        return m_flag.isEnabled.value() ? &bobbyicons::checked : &bobbyicons::unchecked;
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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_RESET_NVS>, ResetNVSAction, StaticMenuItemIcon<&bobbyicons::info>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_REBOOT>, PushScreenAction<RebootScreen>, StaticMenuItemIcon<&bobbyicons::reboot>>>();
}

std::string RecoveryMenu::text() const
{
    return "Recovery Menu";
}
} // namespace bobby
