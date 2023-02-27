#pragma once

// 3rdparty lib includes
#include "actioninterface.h"

// local includes
#include "ledstrip.h"
#include "ledstripdefines.h"
#include "newsettings.h"
#include "guihelpers/bobbyerrorhandler.h"

namespace bobby {

template<LedstripAnimation type>
class LedStripSetAnimationActionStatic : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        if (auto result = configs.write_config(configs.ledstrip.animationType, type); !result)
            BobbyErrorHandler{}.errorOccurred(std::move(result).error());
    }
};

class LedStripSetAnimationAction : public virtual espgui::ActionInterface
{
public:
    LedStripSetAnimationAction(LedstripAnimation animation) : m_animation{animation} {};
    void triggered()
    {
        if (auto result = configs.write_config(configs.ledstrip.animationType, m_animation); !result)
            BobbyErrorHandler{}.errorOccurred(std::move(result).error());
    }
private:
    const LedstripAnimation m_animation;
};

} // namespace bobby
