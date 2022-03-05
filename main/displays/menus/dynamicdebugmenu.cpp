#include "dynamicdebugmenu.h"

// system includes
#include <string>
#include <optional>

// 3rdparty lib includes
#include <espchrono.h>
#include <fmt/core.h>
#include <randomutils.h>
#include <esprandom.h>
#include <menuitem.h>
#include <actions/dummyaction.h>
#include <icons/back.h>
#include <changevaluedisplay.h>
#include <changevaluedisplay_bool.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "utils.h"
#include "icons/lock.h"
#include "bobbycheckbox.h"
#include "bobbyerrorhandler.h"

using namespace espgui;

namespace {
constexpr char TEXT_FONTTEST0[] = "Rich&1Text&2Renderer";
constexpr char TEXT_FONTTEST1[] = "Test&ssmall&mmedium";
constexpr char TEXT_FONTTEST2[] = "Test&ssmall&frestored";
constexpr char TEXT_FONTTEST3[] = "Test&s&3small&4with&6color&7grey";
constexpr char TEXT_DUMMYITEM[] = "Dummy item";
constexpr char TEXT_DYNAMICCOLOR[] = "Dynamic color";
constexpr char TEXT_DYNAMICFONT[] = "Dynamic font";
constexpr char TEXT_DYNAMICICON[] = "Dynamic icon";
constexpr char TEXT_STATICICON[] = "Static icon";
constexpr char TEXT_LOCKTOGGLE[] = "Lock toggle";
constexpr char TEXT_DEBUGTOGGLE[] = "Toggle";
constexpr char TEXT_DEBUGTOGGLEMENU[] = "Toggle (with menu)";
constexpr char TEXT_OPENPOPUP[] = "Open popup";
constexpr char TEXT_BACK[] = "Back";

bool toggleLocked{};
bool toggle{};

class RandomColor : public virtual ColorInterface
{
public:
    int color() const override;

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable int m_color;
};

class RandomFont : public virtual FontInterface
{
public:
    int font() const override;

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable int m_font;
};

class RandomIcon : public virtual MenuItemIconInterface
{
public:
    const MenuItemIcon *icon() const override;

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable const Icon<24, 24> *m_icon;
};

struct ToggleLockedAccessor : public virtual RefAccessor<bool>
{
public:
    bool &getRef() const override { return toggleLocked; }
};

struct ToggleAccessor : public virtual AccessorInterface<bool>
{
public:
    bool getValue() const override { return toggle; };
    tl::expected<void, std::string> setValue(bool value) override;
};

using ToggleChangeValueDisplay = espgui::makeComponent<
    BobbyChangeValueDisplay<bool>,
    espgui::StaticText<TEXT_DEBUGTOGGLEMENU>,
    ToggleAccessor,
    espgui::ConfirmActionInterface<espgui::PopScreenAction>,
    espgui::BackActionInterface<espgui::PopScreenAction>
>;

class OpenPopupAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        BobbyErrorHandler{}.errorOccured("Das\nist\nein sehr langer text, der nicht in eine zeile passt");
    }
};
} // namespace

std::string RandomText::text() const
{
    const auto now = espchrono::millis_clock::now();
    if (!m_nextUpdate || now >= *m_nextUpdate)
    {
        m_title = fmt::format("Dynamic text: {}", cpputils::randomNumber<uint8_t>(100, espcpputils::esp_random_device{}));
        m_nextUpdate = now + std::chrono::milliseconds{cpputils::randomNumber<long>(100, 1000, espcpputils::esp_random_device{})};
    }

    return m_title;
}

DynamicDebugMenu::DynamicDebugMenu()
{
    // dummy items to allow for scrolling
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FONTTEST0>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FONTTEST1>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FONTTEST2>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_FONTTEST3>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();

    // the interesting bits
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_OPENPOPUP>,    OpenPopupAction>>();
    constructMenuItem<makeComponent<MenuItem, RandomText,                    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICCOLOR>, RandomColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICFONT>,  RandomFont, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DYNAMICICON>,  RandomIcon, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STATICICON>,   StaticMenuItemIcon<&bobbyicons::lock>, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_LOCKTOGGLE>,   BobbyCheckbox, ToggleLockedAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEBUGTOGGLE>,  BobbyCheckbox, ToggleAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DEBUGTOGGLEMENU>, PushScreenAction<ToggleChangeValueDisplay>>>();
    constructMenuItem<makeComponent<MenuItem, RandomText,                    RandomColor, RandomFont, RandomIcon, DummyAction>>();

    // more scrolling
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DUMMYITEM>,    DummyAction>>();

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,         PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void DynamicDebugMenu::back()
{
    espgui::popScreen();
}

namespace {

int RandomColor::color() const
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

int RandomFont::font() const
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

const MenuItemIcon *RandomIcon::icon() const
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

tl::expected<void, std::string> ToggleAccessor::setValue(bool value)
{
    if (toggleLocked)
        return tl::make_unexpected("cannot be changed while is locked!");
    toggle = value;
    return {};
}

} // namespace
