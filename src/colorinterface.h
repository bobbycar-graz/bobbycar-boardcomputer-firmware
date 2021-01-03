#pragma once

#include "TFT_eSPI.h"

namespace {
class ColorInterface {
public:
    virtual int color() const { return TFT_WHITE; };
};

class ConstantColor : public virtual ColorInterface
{
public:
    ConstantColor(int color) :
        m_color{color}
    {}

    int color() const override { return m_color; }

private:
    const int m_color;
};

class DefaultColor : public ConstantColor
{
public:
    DefaultColor() :
        ConstantColor(TFT_WHITE)
    {}
};

class DisabledColor : public ConstantColor
{
public:
    DisabledColor() :
        ConstantColor(TFT_DARKGREY)
    {}
};

class ChangeableColor : public virtual ColorInterface
{
public:
    int color() const override { return m_color; }
    void setColor(const int &color) { m_color = color; }

private:
    int m_color;
};
}
