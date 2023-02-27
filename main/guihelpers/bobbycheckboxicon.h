#pragma once

// 3rdparty lib includes
#include <menuitem.h>
#include <accessorinterface.h>

// local includes
#include "icons/checked.h"
#include "icons/unchecked.h"

namespace bobby {
class BobbyCheckboxIcon : public virtual espgui::MenuItemIconInterface, public virtual espgui::AccessorInterface<bool>
{
public:
    const espgui::MenuItemIcon *icon() const override
    {
        return getValue() ? &bobbyicons::checked : &bobbyicons::unchecked;
    }
};
} // namespace bobby
