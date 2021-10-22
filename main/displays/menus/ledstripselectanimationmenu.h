#pragma once

// Local includes
#include "menudisplay.h"
#include "utils.h"
#include "menuitem.h"
#include "ledstrip.h"
#include "ledstripselectanimationmenu.h"
#include "icons/back.h"
#include "texts.h"
#include "actions/dummyaction.h"
#include "actions/ledstripanimationactions.h"
#include "actions/switchscreenaction.h"
#include "ledstrip.h"
#include "ledstripdefines.h"

class currentSelectedAnimationText : public virtual TextInterface { public: std::string text() const override {
        switch (animation_type) {
            case LEDSTRIP_ANIMATION_TYPE_DEFAULTRAINBOW:
                return TEXT_ANIMATION_DEFAULTRAINBOW;
            case LEDSTRIP_ANIMATION_TYPE_BETTERRAINBOW:
                return TEXT_ANIMATION_BETTERRAINBOW;
            case LEDSTRIP_ANIMATION_TYPE_SPEEDSYNCANIMATION:
                return TEXT_ANIMATION_SPEEDSYNCANIMATION;
            case LEDSTRIP_ANIMATION_TYPE_CUSTOMCOLOR:
                return TEXT_ANIMATION_CUSTOMCOLOR;
            default:
                return "Animation Unkown";
        }
    };
};

using namespace espgui;

namespace  {
    class LedstripMenu;
}

namespace  {
    class LedstripSelectAnimationMenu :
            public MenuDisplay,
            public StaticText<TEXT_SELECTANIMATION>,
            public BackActionInterface<SwitchScreenAction<LedstripMenu>>
    {
    public:
        LedstripSelectAnimationMenu()
        {
            constructMenuItem<makeComponent<MenuItem, currentSelectedAnimationText,                  DisabledColor, DummyAction>>();
            constructMenuItem<makeComponent<MenuItem, EmptyText,                                     DummyAction>>();
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_DEFAULTRAINBOW>,     LedStripSetAnimationAction<LEDSTRIP_ANIMATION_TYPE_DEFAULTRAINBOW>>>();
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_BETTERRAINBOW>,      LedStripSetAnimationAction<LEDSTRIP_ANIMATION_TYPE_BETTERRAINBOW>>>();
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_SPEEDSYNCANIMATION>, LedStripSetAnimationAction<LEDSTRIP_ANIMATION_TYPE_SPEEDSYNCANIMATION>>>();
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ANIMATION_CUSTOMCOLOR>,        LedStripSetAnimationAction<LEDSTRIP_ANIMATION_TYPE_CUSTOMCOLOR>>>();
            constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                         SwitchScreenAction<LedstripMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
        }
    };
} // Namespace
