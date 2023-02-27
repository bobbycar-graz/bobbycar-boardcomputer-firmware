#pragma once

// esp-idf includes
#include <esp_log.h>

// 3rd party includes
#include <fmt/core.h>
#include <tftinterface.h>
#include <tl/expected.hpp>
#include <widgets/label.h>

// local includes
#include "guihelpers/bobbydisplay.h"
#include "guihelpers/bobbyerrorhandler.h"
#include "qrimport.h"
#include "screenmanager.h"

namespace bobby {

class QrImportDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    explicit QrImportDisplay(const std::string &nvs_key, espgui::TftInterface &tft) :
        m_statuslabel{5,(tft.height() / 2)-tft.fontHeight(4)},
        m_nvs_key{nvs_key}
    {}

    explicit QrImportDisplay(std::string &&nvs_key, espgui::TftInterface &tft) :
        m_statuslabel{5,(tft.height() / 2)-tft.fontHeight(4)},
        m_nvs_key{std::move(nvs_key)}
    {}

    void initScreen(espgui::TftInterface &tft) override;
    void update() override;
    void redraw(espgui::TftInterface &tft) override;
    void buttonPressed(espgui::Button button) override;

    void start_request();

private:
    bool m_waitingForResult{false};
    bool m_requestStarted{false};
    espgui::Label m_statuslabel;

    tl::expected<std::string, std::string> m_result;
    std::string m_nvs_key;
};

} // namespace bobby
