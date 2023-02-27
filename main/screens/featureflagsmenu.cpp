#include "featureflagsmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>

// local includes
#include "guihelpers/bobbycheckbox.h"
#include "guihelpers/bobbyerrorhandler.h"
#include "guihelpers/bobbypopupdisplay.h"
#include "icons/checked.h"
#include "icons/unchecked.h"
#include "newsettings.h"
#include "taskmanager.h"

namespace bobby {

namespace {
constexpr const char * const TAG = "FEATUREFLAGSMENU";

constexpr char TEXT_FEATUREFLAGS[] = "Feature Flags";
constexpr char TEXT_BACK[] = "Back";
constexpr char TEXT_POPUP[] = "Feature flags have been changed. Please restart the device to apply the changes.";

bool isDirty{false};

void exitFeatureFlagsMenu()
{
    espgui::popScreen();
    if (isDirty)
    {
        BobbyErrorHandler{}.errorOccurred(TEXT_POPUP);
    }
}

class ExitFeatureFlagsMenuAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        exitFeatureFlagsMenu();
    }
};

class FeatureFlagMenuItem : public espgui::MenuItem, public virtual BobbyErrorHandler {
public:
    explicit FeatureFlagMenuItem(ConfiguredFeatureFlag &flag, bool isInitialized) : m_flag{flag}, m_isInitialized{isInitialized} {}
    std::string text() const override
    {
        std::string_view name = m_flag.isEnabled.nvsName();
        constexpr const std::string_view prefix = "f_";
        if (name.ends_with(prefix))
            name.remove_prefix(prefix.size());
        std::string return_name = std::string{name};
        return_name += m_flag.isBeta() ? " (beta)" : "";
        return return_name;
    }

    int color() const override
    {
        if (m_isInitialized)
        {
            return m_flag.isBeta() ? TFT_YELLOW : TFT_GREEN;
        }
        else
        {
            if (m_flag.isEnabled.value())
            {
                return TFT_RED;
            }
            else
                return m_flag.isBeta() ? TFT_ORANGE : TFT_GREY;
        }
    }

    void triggered() override
    {
        if (auto result = m_flag.isEnabled.write(configs.nvs_handle_user, !m_flag.isEnabled.value()); !result)
            errorOccurred(std::move(result).error());
        else
            isDirty = true;
    }

    const espgui::MenuItemIcon *icon() const override
    {
        return m_flag.isEnabled.value() ? &bobbyicons::checked : &bobbyicons::unchecked;
    }
private:
    ConfiguredFeatureFlag &m_flag;
    const bool m_isInitialized;
};
} // namespace

FeatureFlagsMenu::FeatureFlagsMenu()
{
    using namespace espgui;

    configs.callForEveryFeature([&](ConfiguredFeatureFlag &feature){
        const std::string name = feature.getTaskName();
        if (const auto err = checkInitializedByName(name); err)
        {
            constructMenuItem<FeatureFlagMenuItem>(feature, *err);
        }
        else
            constructMenuItem<FeatureFlagMenuItem>(feature, true);
    });
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, ExitFeatureFlagsMenuAction>>();
}

void FeatureFlagsMenu::start()
{
    Base::start();
    isDirty = false;
}

std::string FeatureFlagsMenu::text() const
{
    return TEXT_FEATUREFLAGS;
}

void FeatureFlagsMenu::back()
{
    exitFeatureFlagsMenu();
}

} // namespace bobby
