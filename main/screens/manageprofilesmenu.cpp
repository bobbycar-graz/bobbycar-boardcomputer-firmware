#include "manageprofilesmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>

// local includes
#include "globals.h"
#include "guihelpers/bobbyerrorhandler.h"
#include "icons/back.h"
#include "mainmenu.h"
#include "presets.h"
#include "settingsutils.h"
#include "utils.h"

namespace bobby {

using namespace espgui;

namespace {
constexpr const char * const TAG = "ProfileManager";
constexpr char TEXT_MANAGEPROFILESMENU[] = "Manage Profiles Menu";
constexpr char TEXT_BACK[] = "Back";
} // namespace

class ManageProfileMenuItem : public MenuItem
{
    using Base = MenuItem;
public:
    ManageProfileMenuItem(ManageProfilesMenu &menu, const uint8_t profileIndex) : m_menu{menu}, m_profileIndex{profileIndex} {}
    void copy()
    {
        if (m_mode == CONFIRM_COPY)
        {
            const auto currProfile = settingsPersister.currentlyOpenProfileIndex();
            if (!currProfile)
                return;

            m_menu.unlock();

            settingsutils::switchProfile(m_menu.m_firstIndex);

            if (!settingsPersister.openProfile(m_profileIndex)) // just switch nvs namespace
            {
                BobbyErrorHandler{}.errorOccurred(fmt::format("openProfile({}) failed", m_profileIndex));
                return;
            }
            saveProfileSettings();
            settingsutils::switchProfile(*currProfile);
            m_mode = _DEFAULT;
            m_menu.m_firstIndex = -1;
        }
        else if (m_menu.m_firstIndex == m_profileIndex && m_mode == SELECT_OTHER)
        {
            m_mode = _DEFAULT;
            m_menu.m_firstIndex = -1;
        }
        else if (m_menu.m_firstIndex == -1 && m_mode == _DEFAULT)
        {
            m_mode = SELECT_OTHER;
            m_menu.m_firstIndex = m_profileIndex;
        }
        else if (m_menu.m_firstIndex != -1 && m_menu.m_firstIndex != m_profileIndex)
        {
            m_menu.lock();
            m_mode = CONFIRM_COPY;
            BobbyErrorHandler{}.errorOccurred(
                    fmt::format("Press CONFIRM to COPY from Profile {} to Profile {}", m_menu.m_firstIndex,
                                m_profileIndex));
        }
    }

    void swap()
    {
        if (m_mode == CONFIRM_SWAP)
        {
            const auto currProfile = settingsPersister.currentlyOpenProfileIndex();
            if (!currProfile)
                return;

            m_menu.unlock();

            settingsutils::switchProfile(m_menu.m_firstIndex);
            const ProfileSettings tmp = profileSettings;
            settingsutils::switchProfile(m_profileIndex);
            const ProfileSettings tmp2 = profileSettings;

            profileSettings = tmp;
            saveProfileSettings();
            settingsutils::switchProfile(m_menu.m_firstIndex);
            profileSettings = tmp2;
            saveProfileSettings();

            m_mode = _DEFAULT;
            m_menu.m_firstIndex = -1;
            settingsutils::switchProfile(*currProfile);
        }
        else if (m_menu.m_firstIndex == m_profileIndex && m_mode == SELECT_OTHER)
        {
            m_mode = _DEFAULT;
            m_menu.m_firstIndex = -1;
        }
        else if (m_menu.m_firstIndex == -1 && m_mode == _DEFAULT)
        {
            m_mode = SELECT_OTHER;
            m_menu.m_firstIndex = m_profileIndex;
        }
        else if (m_menu.m_firstIndex != -1 && m_menu.m_firstIndex != m_profileIndex)
        {
            m_menu.lock();
            m_mode = CONFIRM_SWAP;
            BobbyErrorHandler{}.errorOccurred(
                    fmt::format("Press CONFIRM to SWAP Profile {} with Profile {}", m_menu.m_firstIndex,
                                m_profileIndex));
        }
    }

    void clear()
    {
        if (m_mode == CONFIRM_CLEAR)
        {
            const auto currProfile = settingsPersister.currentlyOpenProfileIndex();
            if (!currProfile)
                return;

            m_menu.unlock();
            ESP_LOGI(TAG, "Reseting profile %i...", m_profileIndex);
            settingsutils::switchProfile(m_profileIndex);
            profileSettings = presets::defaultProfileSettings;
            saveProfileSettings();
            m_mode = _DEFAULT;
            settingsutils::switchProfile(*currProfile);
        }
        else if(m_mode == _DEFAULT)
        {
            m_menu.lock();
            m_mode = CONFIRM_CLEAR;
            BobbyErrorHandler{}.errorOccurred("Press CONFIRM to reset Profile or BACK to cancel.");
        }
    }

    std::string text() const override
    {
        switch (m_mode)
        {
        case _DEFAULT:
            return fmt::format("Profile {}", m_profileIndex);
        case CONFIRM_CLEAR:
            return fmt::format("&1Profile {}", m_profileIndex);
        default:
            break;
        }
        return fmt::format("Profile {}", m_profileIndex);
    }

    int color() const override
    {
        if (m_mode == CONFIRM_CLEAR || m_mode == CONFIRM_COPY || m_mode == CONFIRM_SWAP)
            return espgui::TFT_RED;
        else if (m_menu.m_firstIndex == m_profileIndex)
            return espgui::TFT_GREEN;
        else if (m_menu.m_firstIndex != -1)
            return espgui::TFT_GREY;
        else if (m_menu.m_locked)
            return espgui::TFT_DARKGREY;
        return espgui::TFT_WHITE;
    };

    void triggered() override
    {
        switch (m_menu.m_action)
        {
        case Actions::Clear:
            clear();
            break;
        case Actions::Copy:
            copy();
            break;
        case Actions::Swap:
            swap();
            break;
        default:
            break;
        }
    }

    void update() override
    {
        Base::update();
        if (m_mode != _DEFAULT && !m_menu.m_locked && m_menu.m_firstIndex == -1)
            m_mode = _DEFAULT;
    }
private:
    enum Mode : uint8_t {
        _DEFAULT,
        CONFIRM_CLEAR,
        SELECT_OTHER,
        CONFIRM_COPY,
        CONFIRM_SWAP
    } m_mode;
    ManageProfilesMenu &m_menu;
    const uint8_t m_profileIndex;
};

class ManageProfileModeMenuItem : public MenuItem
{
public:
    ManageProfileModeMenuItem(ManageProfilesMenu &menu) : m_menu{menu} {}
    std::string text() const override {
        return fmt::format("&7{}", m_menu.action_text());
    }

    void triggered() override {
        m_menu.m_action = Actions((uint8_t)m_menu.m_action+1);

        if(m_menu.m_action == Actions::__END__)
        {
            m_menu.m_action = Actions(0);
        }
        m_menu.m_firstIndex = -1;
        m_menu.unlock();
    }
private:
    ManageProfilesMenu &m_menu;
};

ManageProfilesMenu::ManageProfilesMenu()
{
    for (uint8_t i = 0; i < 4; i++)
    {
       constructMenuItem<ManageProfileMenuItem>(*this, i);
    }
    constructMenuItem<ManageProfileModeMenuItem>(*this);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, espgui::PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

void ManageProfilesMenu::start()
{
    Base::start();
    m_oldMode = currentMode;
    currentMode = &m_mode;
}

void ManageProfilesMenu::stop()
{
    Base::stop();

    if (currentMode == &m_mode)
    {
        m_mode.stop();
        lastMode = nullptr;
        currentMode = m_oldMode;
    }
}

std::string ManageProfilesMenu::text() const
{
    return TEXT_MANAGEPROFILESMENU;
}

std::string ManageProfilesMenu::action_text() const
{
    return toString(m_action);
}

void ManageProfilesMenu::lock()
{
    m_locked = true;
}

void ManageProfilesMenu::unlock()
{
    m_locked = false;
}

void ManageProfilesMenu::buttonPressed(espgui::Button button)
{
    using namespace espgui;

    switch (button)
    {
    case Button::Left:
    {
        if (!m_locked && m_firstIndex == -1)
        {
            espgui::popScreen();
            return;
        }

        if (m_locked)
        {
            m_locked = false;
        }

        if (m_firstIndex != -1)
        {
            m_firstIndex = -1;
        }
        break;
    }
    case Button::Up:
    case Button::Down:
        if (m_locked)
        {
            return;
        }
    default:
        Base::buttonPressed(button);
        break;
    }
}

} // namespace bobby
