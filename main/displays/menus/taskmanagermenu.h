#pragma once

// local includes
#include "menudisplay.h"
#include "texts.h"

class TaskmanagerMenu :
    public espgui::MenuDisplay,
    public espgui::StaticText<TEXT_TASKMANAGER>
{
public:
    TaskmanagerMenu();
    void back() override;
};
