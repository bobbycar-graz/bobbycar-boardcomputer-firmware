#pragma once

// local includes
#include "globals.h"
#include "textinterface.h"
#include "utils.h"

template<const char *Ttext, typename Ttype, Ttype *Tptr, typename TreturnType, TreturnType (Ttype::*Tmethod)()>
class StatusTextHelper : public virtual espgui::TextInterface
{
  public:
    std::string text() const override
    {
        using ::to_string;
        using std::to_string;
        return Ttext + to_string((Tptr->*Tmethod)());
    }
};

template<const char *Ttext, typename TreturnType, TreturnType (*Tmethod)()>
class StaticStatusTextHelper : public virtual espgui::TextInterface
{
  public:
    std::string text() const override
    {
        using ::to_string;
        using std::to_string;
        return Ttext + to_string(Tmethod());
    }
};
