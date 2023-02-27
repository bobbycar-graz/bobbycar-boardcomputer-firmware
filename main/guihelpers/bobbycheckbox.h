#pragma once

// 3rdparty lib includes
#include <actions/toggleboolaction.h>

// local includes
#include "bobbycheckboxicon.h"
#include "bobbyerrorhandler.h"

namespace bobby {

class BobbyCheckbox :
    public virtual espgui::ToggleBoolAction,
    public virtual BobbyCheckboxIcon,
    public virtual BobbyErrorHandler
{
};

} // namespace bobby
