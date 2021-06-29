#pragma once

#include <array>
#include <algorithm>
#include <functional>
#include <cassert>

#include "display.h"
#include "textinterface.h"
#include "widgets/label.h"
#include "globals.h"
#include "menuitem.h"

namespace {
class MenuDisplay : public Display, public virtual TextInterface
{
public:
    void start() override;
    void initScreen() override;
    void update() override;
    void redraw() override;
    void stop() override;

    void rotate(int offset) override;
    void confirm() override;

    TextInterface *asTextInterface() override { return this; }
    const TextInterface *asTextInterface() const override { return this; }

    MenuDisplay *asMenuDisplay() override { return this; }
    const MenuDisplay *asMenuDisplay() const override { return this; }

    int selectedIndex() const { return m_selectedIndex; }


    std::size_t menuItemCount() const { return m_menuItems.size(); }

    MenuItem& getMenuItem(std::size_t index)
    {
        assert(index < m_menuItems.size());
        return *m_menuItems[index].get();
    }

    const MenuItem& getMenuItem(std::size_t index) const
    {
        assert(index < m_menuItems.size());
        return *m_menuItems[index].get();
    }

    void runForEveryMenuItem(std::function<void(MenuItem&)> &&callback)
    {
        for (const auto &ptr : m_menuItems)
            callback(*ptr);
    }

    void runForEveryMenuItem(std::function<void(const MenuItem&)> &&callback) const
    {
        for (const auto &ptr : m_menuItems)
            callback(*ptr);
    }

    template<typename T, typename... Args>
    T &constructMenuItem(Args&&... args)
    {
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *ptr;
        emplaceMenuItem(std::move(ptr));
        return ref;
    }

    void emplaceMenuItem(std::unique_ptr<MenuItem> &&ptr)
    {
        m_menuItems.emplace_back(std::move(ptr));
    }

    void clearMenuItems()
    {
        m_menuItems.clear();
    }

    std::unique_ptr<MenuItem> takeLastMenuItem()
    {
        assert(!m_menuItems.empty());
        std::unique_ptr<MenuItem> ptr = std::move(m_menuItems.back());
        m_menuItems.pop_back();
        return ptr;
    }

protected:
    void setSelectedIndex(int selectedIndex) { m_selectedIndex = selectedIndex; }

private:
    Label m_titleLabel{5, 5}; // 230, 25

    static constexpr auto iconWidth = 25;
    static constexpr auto horizontalSpacing = 10;
    static constexpr auto topMargin = 40;
    static constexpr auto lineHeight = 25;
    static constexpr auto verticalSpacing = 3;

    std::array<Label, 10> m_labels {{
        Label{horizontalSpacing + iconWidth, topMargin+(0*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(1*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(2*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(3*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(4*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(5*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(6*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(7*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(8*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
        Label{horizontalSpacing + iconWidth, topMargin+(9*(lineHeight+verticalSpacing))}, // 240-(horizontalSpacing*2)-iconWidth, lineHeight
    }};

    std::array<const Icon<24, 24> *, 10> m_icons;

    int m_selectedIndex;
    int m_scrollOffset;
    int m_highlightedIndex;

    int m_rotateOffset;
    bool m_pressed;

    std::vector<std::unique_ptr<MenuItem>> m_menuItems;
};

void MenuDisplay::start()
{
    m_selectedIndex = 0;
    m_scrollOffset = 0;

    m_rotateOffset = 0;
    m_pressed = false;
}

void MenuDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);

    m_titleLabel.start();
    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    for (auto &label : m_labels)
        label.start();

    runForEveryMenuItem([](MenuItem &item){
        item.start();
    });

    m_icons.fill(nullptr);

    m_highlightedIndex = -1;
}

void MenuDisplay::update()
{
    if (!m_pressed)
    {
        const auto offset = m_rotateOffset;
        m_rotateOffset = 0;

        const auto itemCount = menuItemCount();

        if (itemCount)
        {
            if (m_selectedIndex == -1)
                m_selectedIndex = 0;

            m_selectedIndex = m_selectedIndex + offset;

            if (m_selectedIndex < 0)
                m_selectedIndex = 0;
            if (m_selectedIndex >= itemCount)
                m_selectedIndex = itemCount - 1;

            if (m_selectedIndex < m_scrollOffset)
                m_scrollOffset = m_selectedIndex;
            if (m_selectedIndex >= m_scrollOffset + m_labels.size())
                m_scrollOffset = m_selectedIndex - m_labels.size() + 1;
        }
        else
        {
            m_selectedIndex = -1;
            m_scrollOffset = 0;
        }

        runForEveryMenuItem([&](MenuItem &item){
            item.update();
        });
    }
    else
    {
        m_pressed = false;
        if (m_selectedIndex >= 0)
            getMenuItem(m_selectedIndex).triggered();
    }
}

void MenuDisplay::redraw()
{
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    m_titleLabel.redraw(text());

    int i{0};

    auto labelsIter = std::begin(m_labels);

    auto iconsIter = std::begin(m_icons);

    int newHighlightedIndex{-1};

    const auto drawItemRect = [](const auto &label, const auto color){
        tft.drawRect(5,
                     label.y()-1,
                     240 - 10,
                     lineHeight+2,
                     color);
    };

    runForEveryMenuItem([&](MenuItem &item){
        const auto index = i++;

        if (index < m_scrollOffset)
            return;

        if (labelsIter == std::end(m_labels))
            return;

        const auto relativeIndex = index - m_scrollOffset;
        const auto selected = index == m_selectedIndex;

        if (selected)
            newHighlightedIndex = relativeIndex;
        else if (relativeIndex == m_highlightedIndex)
            drawItemRect(*labelsIter, TFT_BLACK);

        tft.setTextFont(item.font());
        tft.setTextColor(item.color(), TFT_BLACK);
        labelsIter->redraw(item.text());

        if (item.icon() != *iconsIter)
        {
            tft.fillRect(5, labelsIter->y()+1, 24, 24, TFT_BLACK);

            auto icon = item.icon();
            if (icon)
            {
                tft.setSwapBytes(!settings.boardcomputerHardware.swapScreenBytes);
                tft.pushImage(6, labelsIter->y()+1, icon->WIDTH, icon->HEIGHT, icon->buffer);
                tft.setSwapBytes(settings.boardcomputerHardware.swapScreenBytes);
            }
            *iconsIter = icon;
        }

        if (selected && (relativeIndex != m_highlightedIndex))
        {
            drawItemRect(*labelsIter, TFT_WHITE);
        }

        labelsIter++;
        iconsIter++;
    });

    for (; labelsIter != std::end(m_labels); labelsIter++, iconsIter++)
    {
        const auto relativeIndex = std::distance(std::begin(m_labels), labelsIter);

        if (relativeIndex == m_highlightedIndex)
            drawItemRect(*labelsIter, TFT_BLACK);

        labelsIter->clear();

        if (*iconsIter)
        {
            tft.fillRect(5, labelsIter->y()+1, 24, 24, TFT_BLACK);
            *iconsIter = nullptr;
        }
    }

    m_highlightedIndex = newHighlightedIndex;
}

void MenuDisplay::stop()
{
    runForEveryMenuItem([](MenuItem &item){
        item.stop();
    });
}

void MenuDisplay::rotate(int offset)
{
    m_rotateOffset += offset;
}

void MenuDisplay::confirm()
{
    m_pressed = true;
}
}
