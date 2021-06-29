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
    public virtual IconInterface<24, 24>
{
public:
    virtual void start() {}
    virtual void update() {}
    virtual void stop() {}
};
}
