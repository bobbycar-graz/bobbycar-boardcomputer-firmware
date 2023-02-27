#pragma once

// system includes
#include <string>

// 3rdparty lib includes
#include <actioninterface.h>

// local includes
#include "screens/qrdisplay.h"
#include "screens/qrimportdisplay.h"

namespace qraction {
struct QrMenu {
    std::string message;
    std::string text;
};
} // namespace

class PushQrDisplayAction : public virtual espgui::ActionInterface
{
public:
    explicit PushQrDisplayAction(const qraction::QrMenu &qrmenu) : m_msg{qrmenu.message} {}
    explicit PushQrDisplayAction(qraction::QrMenu &&qrmenu) : m_msg{std::move(qrmenu.message)} {}

    void triggered() override
    {
        espgui::pushScreen<bobby::QrDisplay>(m_msg);
    }
private:
    std::string m_msg;
};

class PushQrImportDisplayAction : public virtual espgui::ActionInterface
{
public:
    explicit PushQrImportDisplayAction(const std::string &nvskey) : m_nvskey{nvskey} {}
    explicit PushQrImportDisplayAction(std::string &&nvskey) : m_nvskey{std::move(nvskey)} {}

    void triggered() override
    {
        espgui::changeScreenCallback = [&](espgui::TftInterface &tft){
            espgui::pushScreenInternal();

            auto newDisplay = std::make_unique<bobby::QrImportDisplay>(std::move(m_nvskey), tft);
            newDisplay->start();
            newDisplay->initScreen(tft);
            newDisplay->update();
            newDisplay->redraw(tft);
            espgui::currentDisplay = std::move(newDisplay);
        };
    }
private:
    std::string m_nvskey;
};
