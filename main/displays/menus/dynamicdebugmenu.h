#pragma once

// system includes
#include <string>
#include <optional>

// 3rdparty lib includes
#include <espchrono.h>
#include <fmt/core.h>
#include <randomutils.h>
#include <esprandom.h>

// local includes
#include "menudisplay.h"
#include "utilities/utils.h"
#include "menuitem.h"
#include "actions/switchscreenaction.h"
#include "actions/dummyaction.h"
#include "actions/toggleboolaction.h"
#include "icons/lock.h"
#include "checkboxicon.h"
#include "icons/back.h"
#include "texts.h"

using namespace espgui;

namespace {
class RandomText : public virtual TextInterface
{
public:
    std::string text() const override
    {
        const auto now = espchrono::millis_clock::now();
        if (!m_nextUpdate || now >= *m_nextUpdate)
        {
            m_title = fmt::format("Dynamic text: {}", cpputils::randomNumber<uint8_t>(100, espcpputils::esp_random_device{}));
            m_nextUpdate = now + std::chrono::milliseconds{cpputils::randomNumber<long>(100, 1000, espcpputils::esp_random_device{})};
        }

        return m_title;
    }

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable std::string m_title;
};

class RandomColor : public virtual ColorInterface
{
public:
    int color() const override
    {
        const auto now = espchrono::millis_clock::now();
        if (!m_nextUpdate || now >= *m_nextUpdate)
        {
            const auto count = std::size(default_4bit_palette);
            m_color = default_4bit_palette[cpputils::randomNumber<uint32_t>(count-1, espcpputils::esp_random_device{})];
            m_nextUpdate = now + std::chrono::milliseconds{cpputils::randomNumber<long>(100, 1000, espcpputils::esp_random_device{})};
        }

        return m_color;
    }

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable int m_color;
};

class RandomFont : public virtual FontInterface
{
public:
    int font() const override
    {
        const auto now = espchrono::millis_clock::now();
        if (!m_nextUpdate || now >= *m_nextUpdate)
        {
            constexpr const int fonts[] = { 2, 4 };
            m_font = fonts[cpputils::randomNumber<uint32_t>(std::size(fonts)-1, espcpputils::esp_random_device{})];
            m_nextUpdate = now + std::chrono::milliseconds{cpputils::randomNumber<long>(100, 1000, espcpputils::esp_random_device{})};
        }

        return m_font;
    }

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable int m_font;
};

class RandomIcon : public virtual MenuItemIconInterface
{
public:
    const MenuItemIcon *icon() const override
    {
        const auto now = espchrono::millis_clock::now();
        if (!m_nextUpdate || now >= *m_nextUpdate)
        {
            if (m_icon)
                m_icon = nullptr;
            else
                m_icon = &bobbyicons::lock;
            m_nextUpdate = now + std::chrono::milliseconds{cpputils::randomNumber<long>(100, 1000, espcpputils::esp_random_device{})};
        }

        return m_icon;
    }

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable const Icon<24, 24> *m_icon;
};

bool toggle;
struct ToggleAccessor : public virtual RefAccessor<bool>
{
public:
    bool &getRef() const override { return toggle; }
};

constexpr char TEXT_DUMMYITEM[] = "Dummy item";
constexpr char TEXT_DYNAMICCOLOR[] = "Dynamic color";
constexpr char TEXT_DYNAMICFONT[] = "Dynamic font";
constexpr char TEXT_DYNAMICICON[] = "Dynamic icon";
constexpr char TEXT_STATICICON[] = "Static icon";
constexpr char TEXT_DEBUGTOGGLE[] = "Toggle";

class DynamicDebugMenu :
    public MenuDisplay,
    public RandomText,
    public BackActionInterface<SwitchScreenAction<DebugMenu>>
{
public:
    DynamicDebugMenu()
    {
        // dummy items to allow for scrolling
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();

        // the interesting bits
        constructMenuItem<makeComponent<MenuItem, RandomText,                    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICCOLOR>, RandomColor, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICFONT>,  RandomFont, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICICON>,  RandomIcon, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATICICON>,   StaticMenuItemIcon<&bobbyicons::lock>, DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEBUGTOGGLE>,  ToggleBoolAction, CheckboxIcon, ToggleAccessor>>();
        constructMenuItem<makeComponent<MenuItem, RandomText,                    RandomColor, RandomFont, RandomIcon, DummyAction>>();

        // more scrolling
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();

        constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,         SwitchScreenAction<DebugMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
    }
};
} // namespace
