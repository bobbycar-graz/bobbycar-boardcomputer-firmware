#pragma once

// system includes
#include <memory>

// 3rdparty lib includes
#include <display.h>

class AlertDisplay : public espgui::Display
{
public:
    AlertDisplay(std::unique_ptr<Display> &&lastDisplay);

    //void start() override;
    void initScreen() override;
    //void update() override;
    //void redraw() override;
    //void stop() override;
    void confirm() override;
    void back() override;

    void initOverlay();
    void closeOverlay();

private:
    std::unique_ptr<Display> m_lastDisplay;
};
