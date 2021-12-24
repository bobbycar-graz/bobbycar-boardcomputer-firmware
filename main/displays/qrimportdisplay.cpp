constexpr const char * const TAG = "qrimport";
#include "utilities/qrimportdisplay.h"

// 3rd party includes
#include <screenmanager.h>
#include <fmt/format.h>
#include <esp_log.h>

// displays/menus includes
#include "displays/menus/greenpassmenu.h"

// local includes
#include "utilities/qrimport.h"

// m_statuslabel needs redraw

using namespace espgui;

QrImportDisplay::QrImportDisplay(std::string nvs_key) : m_nvs_key{nvs_key} {}

void QrImportDisplay::start()
{
    m_statuslabel.start();
    qrimport::setup_request();
    m_statuslabel.redraw(fmt::format("Request not running."));
}

void QrImportDisplay::back()
{
    if (!qrimport::get_request_running())
    {
        switchScreen<GreenPassMenu>();
    }
}

void QrImportDisplay::confirm()
{
    // start request
    if (!m_confirmLocked)
    {
        if (const auto result = qrimport::start_qr_request(); !result)
        {
            switchScreen<GreenPassMenu>();
        }
        else
            m_confirmLocked = true;
    }
}

void QrImportDisplay::update()
{
    m_expected = qrimport::check_request();
    if (m_expected)
    {
        ESP_LOGI(TAG, "%s", fmt::format("{} => {}", m_nvs_key, *m_expected).c_str());
        if (const auto result = qrimport::set_qr_code(m_nvs_key, *m_expected); !result)
        {
            tft.setTextColor(TFT_RED);
            m_statuslabel.redraw(esp_err_to_name(result.error()));
            tft.setTextColor(TFT_WHITE);
            m_confirmLocked = true;
        }
        else
        {
            switchScreen<GreenPassMenu>();
        }
    }
}

void QrImportDisplay::redraw()
{
    if (qrimport::get_request_running())
    {
        if (!m_expected)
        {
            tft.setTextColor(TFT_RED);
            m_statuslabel.redraw(*m_expected);
            tft.setTextColor(TFT_WHITE);
        }
    }
    else
    {
        m_statuslabel.redraw("Request not running");
    }
}
