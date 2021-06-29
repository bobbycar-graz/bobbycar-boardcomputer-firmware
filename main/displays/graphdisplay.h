#pragma once

#include "display.h"
#include "textinterface.h"
#include "widgets/label.h"
#include "widgets/graph.h"
#include "globals.h"
#include "statistics.h"

namespace {
template<size_t COUNT>
class GraphAccessorInterface
{
public:
    virtual std::array<std::reference_wrapper<const statistics::ContainerType>, COUNT> getBuffers() const = 0;
};

template<typename T>
class SingleGraphAccessor : public virtual GraphAccessorInterface<1>
{
public:
    Graph<200, 1>::Container getBuffers() const
    {
        return {T{}.getBuffer()};
    }
};

template<typename T1, typename T2>
class DualGraphAccessor : public virtual GraphAccessorInterface<2>
{
public:
    Graph<200, 2>::Container getBuffers() const
    {
        return {T1{}.getBuffer(), T2{}.getBuffer()};
    }
};

template<size_t COUNT>
class GraphDisplay :
        public Display,
        public virtual TextInterface,
        public virtual GraphAccessorInterface<COUNT>
{
public:
    void initScreen() override;
    void redraw() override;

private:
    static constexpr int screenHeight = 320, topMargin = 40, bottomMargin = 10, labelOffset = -5;
    static constexpr int graphHeight = screenHeight-topMargin-bottomMargin;

    Label m_titleLabel{5, 5}; // 230, 25

    Graph<200, COUNT> m_graph{0, 40, 270};
};

template<size_t COUNT>
void GraphDisplay<COUNT>::initScreen()
{
    tft.fillScreen(TFT_BLACK);

    m_titleLabel.start();
    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    m_graph.start(static_cast<const GraphAccessorInterface<COUNT> &>(*this).getBuffers());
}

template<size_t COUNT>
void GraphDisplay<COUNT>::redraw()
{
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    m_titleLabel.redraw(text());

    m_graph.redraw(static_cast<const GraphAccessorInterface<COUNT> &>(*this).getBuffers());
}
}
