#pragma once

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "displays/bobbydisplaywithtitle.h"

class SetupFinalInformationDisplay : public virtual BobbyDisplayWithTitle
{
    using Base = BobbyDisplayWithTitle;
public:
    void initScreen(espgui::TftInterface &tft) override;
    void start() override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

    [[nodiscard]] std::string text() const override;
};
