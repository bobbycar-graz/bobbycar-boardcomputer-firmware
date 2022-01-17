#pragma once

#include "actioninterface.h"

class LedstripAnimationBlinkNoneAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

class LedstripAnimationBlinkLeftAction : public virtual espgui::ActionInterface
{
public:
#ifndef LEDSTRIP_WRONG_DIRECTION
    void triggered() override;
#else
    void triggered() override;
#endif
};

class LedstripAnimationBlinkRightAction : public virtual espgui::ActionInterface
{
public:
#ifndef LEDSTRIP_WRONG_DIRECTION
    void triggered() override;
#else
    void triggered() override;
#endif
};

class LedstripAnimationBlinkBothAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};
