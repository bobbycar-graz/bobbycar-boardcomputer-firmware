#pragma once

#include "TFT_eSPI.h"

namespace {
class ColorInterface {
public:
    virtual int color() const { return TFT_WHITE; };
};

template<int TColor>
class StaticColor : public virtual ColorInterface
{
public:
    static constexpr int STATIC_COLOR = TColor;

    int color() const override { return TColor; }
};

using DefaultColor = StaticColor<TFT_WHITE>;
using DisabledColor = StaticColor<TFT_DARKGREY>;

class ChangeableColor : public virtual ColorInterface
{
public:
    int color() const override { return m_color; }
    void setColor(const int &color) { m_color = color; }

private:
    int m_color;
};
}
