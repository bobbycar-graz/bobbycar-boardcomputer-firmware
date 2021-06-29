#pragma once

#include "menuitem.h"
#include "accessorinterface.h"
#include "icons/checked.h"
#include "icons/unchecked.h"

namespace {
class CheckboxIcon : public virtual MenuItemIconInterface, public virtual AccessorInterface<bool>
{
public:
    const MenuItemIcon *icon() const override
    {
        return getValue() ? &icons::checked : &icons::unchecked;
    }
};
}
