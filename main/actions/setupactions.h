#pragma once

// 3rdparty lib includes
#include <actioninterface.h>

namespace bobby {

class PushButtonCalibrateDisplayAction : public virtual espgui::ActionInterface
{
public:
    explicit PushButtonCalibrateDisplayAction(bool early_return);

    void triggered() override;
private:
    const bool m_early_return;
};

class PushPotiCalibrateDisplayAction : public virtual espgui::ActionInterface
{
public:
    explicit PushPotiCalibrateDisplayAction(bool early_return);

    void triggered() override;
private:
    const bool m_early_return;
};
} // namespace bobby
