#pragma once

#include "textinterface.h"
#include "fontinterface.h"
#include "colorinterface.h"
#include "iconinterface.h"
#include "actioninterface.h"

namespace {
using MenuItemIconInterface = IconInterface<24, 24>;

using MenuItemIcon = Icon<24, 24>;

template<const MenuItemIcon *T>
using StaticMenuItemIcon = StaticIcon<24, 24, T>;

class MenuItem :
    public virtual ActionInterface,
    public virtual TextInterface,
    public virtual FontInterface,
    public virtual ColorInterface,
    public virtual MenuItemIconInterface
{
public:
    virtual void start() {}
    virtual void update() {}
    virtual void stop() {}
};


class NewMenuItem : public MenuItem
{
public:
    NewMenuItem() = default;

    NewMenuItem(std::unique_ptr<TextInterface> &&textInterface) :
        m_textInterface{std::move(textInterface)}
    {}

    NewMenuItem(std::unique_ptr<TextInterface> &&textInterface, std::unique_ptr<ActionInterface> &&actionInterface) :
        m_textInterface{std::move(textInterface)},
        m_actionInterface{std::move(actionInterface)}
    {}

    NewMenuItem(std::unique_ptr<TextInterface> &&textInterface, std::unique_ptr<FontInterface> &&fontInterface) :
        m_textInterface{std::move(textInterface)},
        m_fontInterface{std::move(fontInterface)}
    {}

    NewMenuItem(std::unique_ptr<TextInterface> &&textInterface, std::unique_ptr<FontInterface> &&fontInterface, std::unique_ptr<ActionInterface> &&actionInterface) :
        m_textInterface{std::move(textInterface)},
        m_fontInterface{std::move(fontInterface)},
        m_actionInterface{std::move(actionInterface)}
    {}

    NewMenuItem(std::unique_ptr<TextInterface> &&textInterface, std::unique_ptr<FontInterface> &&fontInterface, std::unique_ptr<ColorInterface> &&colorInterface) :
        m_textInterface{std::move(textInterface)},
        m_fontInterface{std::move(fontInterface)},
        m_colorInterface{std::move(colorInterface)}
    {}

    NewMenuItem(std::unique_ptr<TextInterface> &&textInterface, std::unique_ptr<FontInterface> &&fontInterface, std::unique_ptr<ColorInterface> &&colorInterface, std::unique_ptr<ActionInterface> &&actionInterface) :
        m_textInterface{std::move(textInterface)},
        m_fontInterface{std::move(fontInterface)},
        m_colorInterface{std::move(colorInterface)},
        m_actionInterface{std::move(actionInterface)}
    {}

    NewMenuItem(std::unique_ptr<TextInterface> &&textInterface, std::unique_ptr<ActionInterface> &&actionInterface, std::unique_ptr<MenuItemIconInterface> &&menuItemIconInterface) :
        m_textInterface{std::move(textInterface)},
        m_actionInterface{std::move(actionInterface)},
        m_menuItemIconInterface{std::move(menuItemIconInterface)}
    {}

    // TextInterface
    String text() const override { return m_textInterface ? m_textInterface->text() : String{}; }

    // FontInterface
    int font() const override { return m_fontInterface ? m_fontInterface->font() : 4; }

    // ColorInterface
    int color() const override { return m_colorInterface ? m_colorInterface->color() : TFT_WHITE; }

    // ActionInterface
    void triggered() override { if (m_actionInterface) m_actionInterface->triggered(); }

    // MenuItemIconInterface
    const MenuItemIcon *icon() const override { return m_menuItemIconInterface ? m_menuItemIconInterface->icon() : nullptr; }

private:
    std::unique_ptr<TextInterface> m_textInterface;
    std::unique_ptr<FontInterface> m_fontInterface;
    std::unique_ptr<ColorInterface> m_colorInterface;
    std::unique_ptr<ActionInterface> m_actionInterface;
    std::unique_ptr<MenuItemIconInterface> m_menuItemIconInterface;
};
}
