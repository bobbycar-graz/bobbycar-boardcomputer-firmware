#pragma once

// 3rdparty lib includes
#include <textinterface.h>
#include <espchrono.h>

// local includes
#include "displays/bobbymenudisplay.h"

class RandomText : public virtual espgui::TextInterface
{
public:
    std::string text() const override;

private:
    mutable std::optional<espchrono::millis_clock::time_point> m_nextUpdate;
    mutable std::string m_title;
};

class DynamicDebugMenu :
    public BobbyMenuDisplay,
    public RandomText
{
public:
    DynamicDebugMenu();

    void back() override;
};
