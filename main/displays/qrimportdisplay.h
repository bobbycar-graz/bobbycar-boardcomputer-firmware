#pragma once

// system includes
#include <expected>

// 3rd party includes
#include <esp_log.h>
#include <fmt/core.h>
#include <widgets/label.h>

// local includes
#include "bobbydisplay.h"
#include "bobbyerrorhandler.h"
#include "qrimport.h"
#include "screenmanager.h"

class QrImportDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    explicit QrImportDisplay(const std::string &nvs_key) : m_nvs_key{nvs_key} {}
    explicit QrImportDisplay(std::string &&nvs_key) : m_nvs_key{std::move(nvs_key)} {}

    void start() override;
    void update() override;
    void redraw(espgui::TftInterface &tft) override;
    void buttonPressed(espgui::Button button) override;

private:
    bool m_waitingForResult{false};
    espgui::Label m_statuslabel{5,(espgui::tft.height() / 2)-espgui::tft.fontHeight(4)};

    std::expected<std::string, std::string> m_result;
    std::string m_nvs_key;
};
