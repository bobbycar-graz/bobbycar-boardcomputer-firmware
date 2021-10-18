#pragma once

// 3rdparty lib includes
#include "actioninterface.h"

class LedstripAnimationBlinkNoneAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

class LedstripAnimationBlinkLeftAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

class LedstripAnimationBlinkRightAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

class LedstripAnimationBlinkBothAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
