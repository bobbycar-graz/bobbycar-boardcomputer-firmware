#pragma once

#include <string>

#include "utils.h"

namespace {
class TextInterface {
public:
    virtual std::string text() const = 0;
};

class EmptyText : public virtual TextInterface
{
public:
    std::string text() const override { return {}; }
};

template<const char *Ttext>
class StaticText : public virtual TextInterface
{
public:
    static constexpr const char *STATIC_TEXT = Ttext;

    std::string text() const override { return Ttext; }
};

class ChangeableText : public virtual TextInterface
{
public:
    std::string text() const override { return m_title; }
    void setTitle(std::string &&title) { m_title = std::move(title); }
    void setTitle(const std::string &title) { m_title = title; }

private:
    std::string m_title;
};

template<const char *Ttext, typename Ttype, Ttype *Tptr, typename TreturnType, TreturnType (Ttype::*Tmethod)()>
class StatusTextHelper : public virtual TextInterface
{
public:
    std::string text() const override { using std::to_string; using ::to_string; return Ttext + to_string((Tptr->*Tmethod)()); }
};

template<const char *Ttext, typename TreturnType, TreturnType (*Tmethod)()>
class StaticStatusTextHelper : public virtual TextInterface
{
public:
    std::string text() const override { using std::to_string; using ::to_string; return Ttext + to_string(Tmethod()); }
};

template<typename T>
class CachedText : public virtual T
{
public:
    std::string text() const override
    {
        if (!m_loaded)
        {
            m_text = T::text();
            m_loaded = true;
        }

        return m_text;
    }

private:
    mutable bool m_loaded{};
    mutable std::string m_text;
};

template<typename T>
class StaticallyCachedText : public virtual T
{
public:
    std::string text() const override
    {
        static const auto text = T::text();
        return text;
    }
};
}
