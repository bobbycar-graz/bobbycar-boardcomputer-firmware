#pragma once

namespace {
class FontInterface {
public:
    virtual int font() const { return 4; };
};

template<int TFont>
class StaticFont : public virtual FontInterface
{
public:
    static constexpr int STATIC_FONT = TFont;

    int font() const override { return TFont; }
};

using DefaultFont = StaticFont<4>;

class ChangeableFont : public virtual FontInterface
{
public:
    int font() const override { return m_font; }
    void setfont(const int &font) { m_font = font; }

private:
    int m_font;
};
}
