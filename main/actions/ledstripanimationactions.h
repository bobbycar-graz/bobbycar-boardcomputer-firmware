#pragma once

// 3rdparty lib includes
#include "actioninterface.h"

// local includes
#include "ledstrip.h"
#include "ledstripdefines.h"
#include "newsettings.h"
#include "bobbyerrorhandler.h"

template<LedstripAnimation type>
class LedStripSetAnimationAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override;
};

template<LedstripAnimation type>
void LedStripSetAnimationAction<type>::triggered()
{
    if (auto result = configs.write_config(configs.ledstrip.animationType, type); !result)
        BobbyErrorHandler{}.errorOccured(std::move(result).error());
}
